#pragma once

#include "PrimitiveShape.h"
#include "ShaderManager.h"

#include <vector>

class Grid : public PrimitiveShape
{
public:
	Grid(int HalfWidth, std::string ShaderName, glm::vec3 Color = glm::vec3(1.0,1.0,1.0))
	{
		_Shader = ShaderName;
		std::vector<float> vertices;
		for (int x = -HalfWidth; x <= HalfWidth; x++) {
			glm::vec3 first = glm::vec3((float)x, 0.0, (float)-HalfWidth);
			glm::vec3 second = glm::vec3((float)x, 0.0, (float)HalfWidth);
			vertices.push_back(first.x); vertices.push_back(first.y); vertices.push_back(first.z);
			vertices.push_back(second.x); vertices.push_back(second.y); vertices.push_back(second.z);
		}
		for (int z = -HalfWidth; z <= HalfWidth; z++) {
			glm::vec3 first = glm::vec3((float)-HalfWidth, 0.0, (float)z);
			glm::vec3 second = glm::vec3((float)HalfWidth, 0.0, (float)z);
			vertices.push_back(first.x); vertices.push_back(first.y); vertices.push_back(first.z);
			vertices.push_back(second.x); vertices.push_back(second.y); vertices.push_back(second.z);
		}
		_VertexArray.Create(VAO);
		_VertexBuffer.Create(VBO);
		_VertexArray.Bind();
		_VertexBuffer.Fill(vertices.size() * sizeof(float), &vertices[0], STATIC);
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aPos", 3, VT_FLOAT);
		_Color = Color;

		_DrawCount = (((HalfWidth * 2) + 1) * 2) * 2; //this works do not fix this!! see david for details! lol
		_VertexArray.Unbind();
	}

	~Grid()
	{
		_VertexArray.Destroy();
		_VertexBuffer.Destroy();
	}

	void Draw()
	{
		ShaderManager::Instance()->GetShader(_Shader)->SetVec3("aColor", _Color);
		_VertexArray.Bind();
		glDrawArrays(GL_LINES, 0, _DrawCount);
		_VertexArray.Unbind();
	}

private:
	glm::vec3 _Color;

};

