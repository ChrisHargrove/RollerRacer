#pragma once

#include <GLM\glm.hpp>
#include <string>
#include <vector>

#include "Buffer.h"

struct ComplexVertex {
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

	bool _HasTexture;
	glm::vec3 _Color;
};

class Model;

class Mesh
{
public:

    Mesh(std::vector<ComplexVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures, std::string shader);

	friend class Model;

    void Render(std::string shader = "");

protected:
	void SetShininess(float value);

private:
    Buffer _VertexArray;
    Buffer _VertexBuffer;
    Buffer _ElementBuffer;

    std::vector<ComplexVertex> _Vertices;
    std::vector<unsigned int> _Indices;
    std::vector<MeshTexture> _Textures;

    std::string _Shader;
	float _Shininess;
};

