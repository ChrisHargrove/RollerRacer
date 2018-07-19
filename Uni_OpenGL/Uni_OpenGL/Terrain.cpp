#include "Terrain.h"
#include "ShaderManager.h"

#include "PRNG.h"
#include "CatmullRomSpline.h"

Terrain::Terrain(int gridX, int gridZ, std::vector<Texture*> textures, Texture* blendmap)
{
    X = gridX * SIZE;
    Z = gridZ * SIZE;
    _Textures = textures;
    _BlendMap = blendmap;
    _Shader = "terrain";
}

Terrain::~Terrain()
{
    _VertexArray.Destroy();
    _VertexBuffer.Destroy();
    _ElementBuffer.Destroy();

	if (_IndexArray) delete _IndexArray;
}

void Terrain::Render(std::string shader)
{
	if (shader != "") {
		_VertexArray.Bind();
		_VertexBuffer.Bind();
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(shader)->GetID(), "aPos", 3, VT_FLOAT, 10 * sizeof(float));
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(shader)->GetID(), "aNormal", 3, VT_FLOAT, 10 * sizeof(float), 3 * sizeof(float));
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(shader)->GetID(), "aTexCoords", 2, VT_FLOAT, 10 * sizeof(float), 6 * sizeof(float));
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(shader)->GetID(), "aTexData", 2, VT_FLOAT, 10 * sizeof(float), 8 * sizeof(float));
	}
    if (_BlendMap != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        ShaderManager::Instance()->GetShader(_Shader)->SetInt("blendMap", 0);
        glBindTexture(GL_TEXTURE_2D, _BlendMap->GetID());
    }
    for (unsigned int i = 0; i < _Textures.size(); i++) {
        if (_Textures[i] != nullptr) {
            glActiveTexture(GL_TEXTURE1 + i);
            ShaderManager::Instance()->GetShader(_Shader)->SetInt(("material.texture_diffuse" + std::to_string(i + 1)).c_str(), (int)i + 1);
            glBindTexture(GL_TEXTURE_2D, _Textures[i]->GetID());
        }
    }
    ShaderManager::Instance()->GetShader(_Shader)->SetFloat("material.shininess", 1.0f);

    _VertexArray.Bind();
    glDrawElements(GL_TRIANGLES, _Indices.size(), GL_UNSIGNED_INT, 0);
    _VertexArray.Unbind();

    glActiveTexture(GL_TEXTURE0);
}

float Terrain::GetHeight(int x, int z)
{
    return _HeightList[x * VERTEX_COUNT + z];
}

void Terrain::Initialize(btDynamicsWorld * world, std::set<btCollisionShape*>* collisionShapes)
{
    _IndexArray = new btTriangleIndexVertexArray(
        (int)_Indices.size() / 3,
        (int*)&_Indices[0],
        (int)(3 * sizeof(unsigned int)),
        (int)_Vertices.size(),
        (btScalar*)&_Vertices[0],
        (int)sizeof(Vertex));

    _MeshShape = new btBvhTriangleMeshShape(_IndexArray, true, btVector3(0, 0, 0), btVector3(btScalar(SIZE), btScalar(MAX_HEIGHT), btScalar(SIZE)));
    //increase the collision margin so things dont fall through the floor
    //much googling required to sort this haha.
    _MeshShape->setMargin(btScalar(0.1f));

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(-SIZE * 0.5f, 0, -SIZE * 0.5f));

    _MotionState = new OpenGLMotionState(transform);

    //calculate local inertia
    btVector3 localInertia(0, 0, 0);

    btRigidBody::btRigidBodyConstructionInfo bodyInfo(0, _MotionState, _MeshShape, localInertia);

    _TerrainBody = new btRigidBody(bodyInfo);

	collisionShapes->emplace(_MeshShape);
    world->addRigidBody(_TerrainBody);
}

void Terrain::AddSplineToTerrain(CatmullRomSpline * spline)
{
	std::vector<glm::vec2> points = spline->GetSpline();

	for (auto p : points) {
		//put the point into the terrain co-ord system 0-800;
		//but hallf it so points stay centered.
		p += glm::vec2(SIZE * 0.5f);

		//find the nearest terrain vertex to the spline point.
		float spaceBetweenVerts = (float)SIZE / (float)VERTEX_COUNT;
		glm::vec2 np = glm::vec2(p / spaceBetweenVerts);
		np.x = roundf(np.x);
		np.y = roundf(np.y);

		//find all surrounding vertices
		int index = (int)np.x * VERTEX_COUNT + ((int)np.y);
		int left = (int)np.x * VERTEX_COUNT + ((int)np.y - 1);
		int right = (int)np.x * VERTEX_COUNT + ((int)np.y + 1);
		int up = ((int)np.x + 1) * VERTEX_COUNT + ((int)np.y);
		int down = ((int)np.x - 1) * VERTEX_COUNT + ((int)np.y);

		int leftUp = ((int)np.x + 1)  * VERTEX_COUNT + ((int)np.y - 1);
		int rightUp = ((int)np.x + 1) * VERTEX_COUNT + ((int)np.y + 1);
		int leftDown = ((int)np.x - 1) * VERTEX_COUNT + ((int)np.y - 1);
		int rightDown = ((int)np.x - 1) * VERTEX_COUNT + ((int)np.y + 1);

		//change all vertex texture data to use the road texture.
		//first number is texture to use, second is blend amount.
		//lower blend amounts show road more clearly.
		_Vertices[index]._TexData = glm::vec2(1, 0.25);
		_Vertices[left]._TexData = glm::vec2(1, 0.25);
		_Vertices[right]._TexData = glm::vec2(1,0.25);
		_Vertices[up]._TexData = glm::vec2(1, 0.25);
		_Vertices[down]._TexData = glm::vec2(1, 0.25);

		_Vertices[leftUp]._TexData = glm::vec2(1, 0.25);
		_Vertices[rightUp]._TexData = glm::vec2(1, 0.25);
		_Vertices[leftDown]._TexData = glm::vec2(1, 0.25);
		_Vertices[rightDown]._TexData = glm::vec2(1, 0.25);

		//smooth the roads, and lower the height.
		float averageHeight = 0.0f;
		_Vertices[index]._Position.y = 1.0f;
		_Vertices[left]._Position.y = 1.0f;
		_Vertices[right]._Position.y = 1.0f;
		_Vertices[up]._Position.y = 1.0f;
		_Vertices[down]._Position.y = 1.0f;
		_Vertices[leftUp]._Position.y = 1.0f;
		_Vertices[rightUp]._Position.y = 1.0f;
		_Vertices[leftDown]._Position.y = 1.0f;
		_Vertices[rightDown]._Position.y = 1.0f;
		
	}
}

void Terrain::GenerateTerrain()
{
    PRNG _NumberGenerator(_Seed, 0, 5);

    //Total number of vertices
    int count = VERTEX_COUNT * VERTEX_COUNT;

    //reserve memory for the vectors ahead of time
    _HeightList.reserve(count);
    _Vertices.reserve(count);
    _Indices.reserve(6 * (VERTEX_COUNT - 1)*(VERTEX_COUNT - 1));

    //Generate all height values so that they can be smoothed before added to vertices.
    for (int i = 0; i < count; i++) {
        _HeightList.push_back(_NumberGenerator.GetNumberF());
    }

    //smooth the vertices, dont smooth the edges
    for (int x = 1; x < VERTEX_COUNT - 1; x++) {
        for (int z = 1; z < VERTEX_COUNT - 1 ; z++) {

            float center = _HeightList[(x * VERTEX_COUNT) + z];

            float left = _HeightList[(x * VERTEX_COUNT) + (z - 1)];
            float right = _HeightList[(x * VERTEX_COUNT) + (z + 1)];
            float up = _HeightList[(x + 1) * VERTEX_COUNT + z];
            float down = _HeightList[(x - 1) * VERTEX_COUNT + z];

            float leftUp = _HeightList[((x + 1) * VERTEX_COUNT) + (z - 1)];
            float leftDown = _HeightList[((x - 1) * VERTEX_COUNT) + (z - 1)];
            float rightUp = _HeightList[((x + 1) * VERTEX_COUNT) + (z + 1)];
            float rightDown = _HeightList[((x - 1) * VERTEX_COUNT) + (z+ 1)];;

            float totalHeight = center + left + leftUp + up + rightUp + right + rightDown + down + leftDown;
            totalHeight /= 9.0f;

            //should now be a smoother height transition;
            _HeightList[x * VERTEX_COUNT + z] = totalHeight;

        }
    }
}

void Terrain::FormatVertexData()
{
    //assign values to each vertex
    for (int x = 0; x<VERTEX_COUNT; x++) {
        for (int z = 0; z<VERTEX_COUNT; z++) {
            Vertex temp;

            temp._Position = glm::vec3((float)x / ((float)VERTEX_COUNT - 1) * SIZE,
                _HeightList[z * VERTEX_COUNT + x], (float)z / ((float)VERTEX_COUNT - 1) * SIZE);

            temp._UV = glm::vec2((float)x / ((float)VERTEX_COUNT - 1),
                (float)z / ((float)VERTEX_COUNT - 1));

			temp._TexData = glm::vec2(0, 1);

            _Vertices.push_back(temp);
        }
    }
    //add all indice data so it draws correctly
    for (int x = 0; x<VERTEX_COUNT - 1; x++) {
        for (int z = 0; z<VERTEX_COUNT - 1; z++) {
            int topLeft = (x * VERTEX_COUNT) + z; //bottomleft
            int topRight = topLeft + 1; //topleft
            int bottomLeft = ((x + 1) * VERTEX_COUNT) + z; //bottom right
            int bottomRight = bottomLeft + 1; //topright

			_Indices.push_back(topLeft);//bottomleft
            _Indices.push_back(bottomLeft);//bottom right
            _Indices.push_back(topRight);//topleft
            _Indices.push_back(bottomRight);//topright
            _Indices.push_back(topRight);//topleft
            _Indices.push_back(bottomLeft);//bottom right
        }
    }
}

void Terrain::AddToBuffer()
{
	_VertexArray.Create(VAO);
	_VertexBuffer.Create(VBO);
	_ElementBuffer.Create(EBO);
	_VertexArray.Bind();
	_VertexBuffer.Fill(sizeof(Vertex) * _Vertices.size(), &_Vertices[0], STATIC);
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aPos", 3, VT_FLOAT, 10 * sizeof(float));
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aNormal", 3, VT_FLOAT, 10 * sizeof(float), 3 * sizeof(float));
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aTexCoords", 2, VT_FLOAT, 10 * sizeof(float), 6 * sizeof(float));
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aTexData", 2, VT_FLOAT, 10 * sizeof(float), 8 * sizeof(float));

	_ElementBuffer.Fill(sizeof(unsigned int) * _Indices.size(), &_Indices[0], STATIC);
	_VertexArray.Unbind();
}

void Terrain::CalculateNormals()
{
	std::vector<glm::vec3> vertexNormals;
	vertexNormals.reserve(_Vertices.size());
	int triangleCount = _Indices.size() / 3;
	for (int i = 0; i < triangleCount; i++) {
		vertexNormals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	for (int i = 0; i < triangleCount; i++) {
		int normalTriangleIndex = i * 3;
		int vertIndexA = _Indices[normalTriangleIndex];
		int vertIndexB = _Indices[normalTriangleIndex + 1];
		int vertIndexC = _Indices[normalTriangleIndex + 2];

		glm::vec3 triangleNormal = SurfaceNormalFromIndices(vertIndexA, vertIndexB, vertIndexC);
		vertexNormals[vertIndexA] += triangleNormal;
		vertexNormals[vertIndexB] += triangleNormal;
		vertexNormals[vertIndexC] += triangleNormal;
	}

	for (int i = 0; i < vertexNormals.size(); i++) {
		vertexNormals[i] = glm::normalize(vertexNormals[i]);
	}

	for (int i = 0; i < _Vertices.size(); i++) {
		_Vertices[i]._Normal = vertexNormals[i];
	}
}

glm::vec3 Terrain::SurfaceNormalFromIndices(int a, int b, int c)
{
	glm::vec3 pointA = _Vertices[a]._Position;
	glm::vec3 pointB = _Vertices[b]._Position;
	glm::vec3 pointC = _Vertices[c]._Position;

	glm::vec3 sideAB = glm::normalize(pointB - pointA);
	glm::vec3 sideAC = glm::normalize(pointC - pointA);

	return glm::normalize(glm::cross(sideAC, sideAB));
}
