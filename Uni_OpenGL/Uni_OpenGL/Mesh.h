#pragma once

#include <GLM\glm.hpp>
#include <string>
#include <vector>

#include "Buffer.h"

struct Vertex {
    glm::vec3 _Position;
    glm::vec3 _Normal;
    glm::vec2 _UV;
    glm::vec3 _Tangent;
    glm::vec3 _Bitangent;
};


struct MeshTexture {
    unsigned int _ID;
    std::string _Type;
    std::string _Path;
};

class Mesh
{
public:

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures, std::string shader);

    void Draw();

private:
    Buffer _VertexArray;
    Buffer _VertexBuffer;
    Buffer _ElementBuffer;

    std::vector<Vertex> _Vertices;
    std::vector<unsigned int> _Indices;
    std::vector<MeshTexture> _Textures;

    std::string _Shader;
};

