#pragma once

#include "PrimitiveShape.h"
#include "Texture.h"

#include <set>
#include <vector>
#include <string>
#include <GLM\glm.hpp>
#include <BULLET\btBulletCollisionCommon.h>
#include <BULLET\btBulletDynamicsCommon.h>
#include "OpenGLMotionState.h"

class CatmullRomSpline;

struct Vertex {
    glm::vec3 _Position;
    glm::vec3 _Normal;
    glm::vec2 _UV;
	glm::vec2 _TexData;
};

class Terrain : public PrimitiveShape
{
public:
    Terrain(int gridX, int gridZ, std::vector<Texture*> textures, Texture* blendmap);
    ~Terrain();

    // Inherited via PrimitiveShape
    virtual void Render(std::string shader = "") override;

    float GetHeight(int x, int z);
	float GetSize() { return SIZE; }
	int GetVertexCount() { return VERTEX_COUNT; }
	float GetVertexSpacing() { return SIZE / VERTEX_COUNT; }

    void SetSeed(int seed) { _Seed = seed; }

    void Initialize(btDynamicsWorld* world, std::set<btCollisionShape*>* collisionShapes);

	btRigidBody* GetRigidBody() { return _TerrainBody; }

    glm::mat4 GetModelMatrix() {
        if (_MotionState) { return _MotionState->GetWorldMatrix(); }
        else { return glm::mat4(1.0f); }
    }

	void AddSplineToTerrain(CatmullRomSpline* spline);
	void CalculateNormals();

    void GenerateTerrain();
    void FormatVertexData();
	void AddToBuffer();
private:
    const float SIZE = 800;
    const int VERTEX_COUNT = 128;
    const int MAX_HEIGHT = 100;

    float X;
    float Z;
    int _Seed = 0;

    std::vector<Texture*> _Textures;
    Texture*    _BlendMap;
    Buffer  _ElementBuffer;
    std::string _Shader;

    btTriangleIndexVertexArray* _IndexArray;
    btBvhTriangleMeshShape* _MeshShape;

    btRigidBody*    _TerrainBody;
    OpenGLMotionState*  _MotionState;

    std::vector<float> _HeightList;

    std::vector<Vertex> _Vertices;
    std::vector<unsigned int> _Indices;

	glm::vec3 SurfaceNormalFromIndices(int a, int b, int c);
};