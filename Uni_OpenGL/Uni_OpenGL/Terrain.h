#pragma once

#include "PrimitiveShape.h"

class Terrain : public PrimitiveShape
{
    Terrain();
    Terrain(glm::vec2 Position, std::string Shader);
    ~Terrain();

    void Render();
    glm::vec2* GetPosition();

protected:
    void GenerateTerrain();

private:
    glm::vec2 _Position;
    Buffer _ElementBuffer;

    const float SIZE = 800;
    const int VERTEX_COUNT = 128;
};
























//#include "Mesh.h"
//#include "Texture.h"



//class Terrain
//{
//public:
//    Terrain(int gridX, int gridZ, Texture* texture);
//    ~Terrain();
//
//    void Render();
//    float x;
//    float z;
//
//protected:
//    Mesh* generateTerrain();
//    
//private:
//    const float SIZE = 800;
//    const int VERTEX_COUNT = 128;
//
//    Mesh* mesh;
//    Texture* _texture;
//};

