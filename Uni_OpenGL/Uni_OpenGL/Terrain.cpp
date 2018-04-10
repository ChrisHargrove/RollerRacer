#include "Terrain.h"
#include "ShaderManager.h"
#include <vector>

#include <iostream>

//Terrain::Terrain(int gridX, int gridZ, Texture * texture)
//{
//    x = gridX * SIZE;
//    z = gridZ * SIZE;
//    _texture = texture;
//    mesh = generateTerrain();
//}
//
//Terrain::~Terrain()
//{
//}
//
//void Terrain::Render()
//{
//    mesh->Render();
//}
//
//Mesh* Terrain::generateTerrain() {
//    int count = VERTEX_COUNT * VERTEX_COUNT;
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<MeshTexture> textures;
//
//    for (int i = 0; i<VERTEX_COUNT; i++) {
//        for (int j = 0; j<VERTEX_COUNT; j++) {
//
//            Vertex vert;
//            vert._Position.x = (float)j / ((float)VERTEX_COUNT) * SIZE;
//            vert._Position.y = rand() % 2;
//            vert._Position.z = (float)i / ((float)VERTEX_COUNT) * SIZE;
//
//            vert._Normal.x = 0;
//            vert._Normal.y = 1;
//            vert._Normal.z = 0;
//
//            vert._UV.x = (float)j / ((float)VERTEX_COUNT - 1);
//            vert._UV.y = (float)i / ((float)VERTEX_COUNT - 1);
//
//            vertices.push_back(vert);
//        }
//    }
//    int pointer = 0;
//    for (int gz = 0; gz<VERTEX_COUNT - 1; gz++) {
//        for (int gx = 0; gx<VERTEX_COUNT - 1; gx++) {
//            int topLeft = (gz*VERTEX_COUNT) + gx;
//            int topRight = topLeft + 1;
//            int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
//            int bottomRight = bottomLeft + 1;
//            indices.push_back(topLeft);
//            indices.push_back(bottomLeft);
//            indices.push_back(topRight);
//            indices.push_back(topRight);
//            indices.push_back(bottomLeft);
//            indices.push_back(bottomRight);
//        }
//    }
//    MeshTexture newMeshTexture;
//    newMeshTexture._ID = _texture->GetID();
//    newMeshTexture._Type = "texture_diffuse";
//    textures.push_back(newMeshTexture);
//
//
//    Mesh* newMesh = new Mesh(vertices, indices, textures, "betterLight");
//    return newMesh;
//}

Terrain::Terrain()
{
}

Terrain::Terrain(glm::vec2 Position, std::string Shader)
{
    _Shader = Shader;
    _Position = Position * SIZE;
}

Terrain::~Terrain()
{
}

void Terrain::Render()
{
}

glm::vec2* Terrain::GetPosition()
{
    return &_Position;
}

void Terrain::GenerateTerrain()
{
}
