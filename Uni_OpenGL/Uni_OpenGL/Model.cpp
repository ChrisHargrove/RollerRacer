#include "Model.h"
#include "LogManager.h"
#include "Texture.h"
#include <SDL\SDL_image.h>
#include <GLEW\glew.h>

Model::Model(std::string FileName, std::string shader)
{
    _Shader = shader;
    LoadModel(FileName);
}

Model::~Model()
{
}

void Model::Render(std::string shader)
{
	//go through each mesh and render it
    for (int i = 0; i < (int)_Meshes.size(); i++) {
        _Meshes[i].Render(shader);
    }
}

std::vector<glm::vec3> Model::GetVertices()
{
    std::vector<glm::vec3> vertices;
    for (auto m : _Meshes) {
        for (auto v : m._Vertices) {
            vertices.push_back(v._Position);
        }
    }
    return vertices;
}

std::vector<unsigned int> Model::GetIndices()
{
    std::vector<unsigned int> indices;
    for (auto m : _Meshes) {
        for (auto i : m._Indices) {
            indices.push_back(i);
        }
    }
    return indices;
}

void Model::LoadModel(std::string FileName)
{
	//uses Assimp library to load all models
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(FileName, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LogManager::Instance()->LogError("[ASSIMP]" + std::string(importer.GetErrorString()));
        return;
    }

	//recursively loads all meshes for the model
    _Directory = FileName.substr(0, FileName.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);

}

void Model::ProcessNode(aiNode * node, const aiScene * scene)
{

    for (int i = 0; i < (int)node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _Meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (int i = 0; i < (int)node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
    std::vector<ComplexVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;
	float shininess;

    for (int i = 0; i < (int)mesh->mNumVertices; i++) {
        ComplexVertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex._Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex._Normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex._UV = vec;
        }
        else {
            vertex._UV = glm::vec2(0.0f, 0.0f);
        }

        if (mesh->mTangents != NULL) {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex._Tangent = vector;
        }
        if (mesh->mBitangents != NULL) {
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex._Bitangent = vector;
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
		//load all material data from mtl files
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<MeshTexture> diffuseMaps = LoadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<MeshTexture> specularMaps = LoadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<MeshTexture> normalMaps = LoadMaterialTextures(material, 
            aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<MeshTexture> heightMaps = LoadMaterialTextures(material, 
            aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininess)) {
			LogManager::Instance()->LogWarning("Material Found Without Shininess Value");
		}
    }

	Mesh returnMesh(vertices, indices, textures, _Shader);
	returnMesh.SetShininess(shininess);

    return returnMesh;
}

std::vector<MeshTexture> Model::LoadMaterialTextures(aiMaterial * material, aiTextureType type, std::string typeName)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        MeshTexture texture;
		if (aiReturn_SUCCESS != material->GetTexture(type, i, &str)) {
		}
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < _Textures.size(); j++)
        {
            if (std::strcmp(_Textures[j]._Path.data(), str.C_Str()) == 0)
            {
                textures.push_back(_Textures[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it

            texture._ID = TextureFromFile(_Directory + "/" + std::string(str.C_Str()));
            texture._Type = typeName;
            texture._Path = str.C_Str();
            textures.push_back(texture);
            _Textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(std::string FileName)
{
    unsigned int _ID;
    SDL_Surface* image;
    //check to see if image was found
    if ((image = IMG_Load(FileName.c_str()))) {
        //check to see if image was loaded
        if (!image) {
            LogManager::Instance()->LogError("Didnt load image " + FileName);
            return false;
        }

        //This bit makes using image formats with transparency possible
        GLenum format;
        if (image->format->Amask > 0) {
            format = GL_RGBA;
        }
        else {
            format = GL_RGB;
        }

        //create opengl texture
        glGenTextures(1, &_ID);
        //bind texture
        glBindTexture(GL_TEXTURE_2D, _ID);
        //set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Linear Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Generate The Texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
        //generate a mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        if (image) { //free any memory that may have been used.
            SDL_FreeSurface(image);
        }
        LogManager::Instance()->LogInfo("Texture " + FileName + " Loaded...");

        return _ID;
    }
    else {
        LogManager::Instance()->LogError("Texture " + FileName + " Failed...");
        return 0;
    }
}

