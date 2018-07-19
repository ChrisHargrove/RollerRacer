#pragma once

#include "PrimitiveShape.h"
#include "ShaderManager.h"

#include <vector>

class Cuboid : public PrimitiveShape
{
public:
	Cuboid(float Width, float Height, float Depth, std::string ShaderName, glm::vec3 Color = glm::vec3(1.0, 1.0, 1.0))
	{
		_Shader = ShaderName;

		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f * Width, -0.5f * Height, -0.5f * Depth,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			0.5f * Width, -0.5f * Height, -0.5f * Depth,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			0.5f * Width,  0.5f * Height, -0.5f * Depth,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			0.5f * Width,  0.5f * Height, -0.5f * Depth,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f * Width,  0.5f * Height, -0.5f * Depth,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f * Width, -0.5f * Height, -0.5f * Depth,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f * Width, -0.5f * Height,  0.5f * Depth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			0.5f * Width, -0.5f * Height,  0.5f * Depth,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			0.5f * Width,  0.5f * Height,  0.5f * Depth,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			0.5f * Width,  0.5f * Height,  0.5f * Depth,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f * Width,  0.5f * Height,  0.5f * Depth,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f * Width, -0.5f * Height,  0.5f * Depth,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f * Width,  0.5f * Height,  0.5f * Depth, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f * Width,  0.5f * Height, -0.5f * Depth, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f * Width, -0.5f * Height, -0.5f * Depth, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f * Width, -0.5f * Height, -0.5f * Depth, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f * Width, -0.5f * Height,  0.5f * Depth, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f * Width,  0.5f * Height,  0.5f * Depth, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			0.5f * Width,  0.5f * Height,  0.5f * Depth,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			0.5f * Width,  0.5f * Height, -0.5f * Depth,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			0.5f * Width, -0.5f * Height, -0.5f * Depth,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			0.5f * Width, -0.5f * Height, -0.5f * Depth,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			0.5f * Width, -0.5f * Height,  0.5f * Depth,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			0.5f * Width,  0.5f * Height,  0.5f * Depth,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f * Width, -0.5f * Height, -0.5f * Depth,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			0.5f * Width, -0.5f * Height, -0.5f * Depth,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			0.5f * Width, -0.5f * Height,  0.5f * Depth,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			0.5f * Width, -0.5f * Height,  0.5f * Depth,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f * Width, -0.5f * Height,  0.5f * Depth,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f * Width, -0.5f * Height, -0.5f * Depth,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f * Width,  0.5f * Height, -0.5f * Depth,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			0.5f * Width,  0.5f * Height, -0.5f * Depth,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			0.5f * Width,  0.5f * Height,  0.5f * Depth,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			0.5f * Width,  0.5f * Height,  0.5f * Depth,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f * Width,  0.5f * Height,  0.5f * Depth,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f * Width,  0.5f * Height, -0.5f * Depth,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		};

		_VertexArray.Create(VAO);
		_VertexBuffer.Create(VBO);
		//_ElementBuffer.Create(EBO);
		_VertexArray.Bind();
		_VertexBuffer.Fill(sizeof(vertices), vertices, STATIC);
		//_ElementBuffer.Fill(sizeof(indices), indices, STATIC);

		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aPos", 3, VT_FLOAT, 8 * sizeof(float));
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aNormal", 3, VT_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aTexCoords", 2, VT_FLOAT, 8 * sizeof(float), 6 * sizeof(float));
		_Color = Color;

		_DrawCount = 36;
		_VertexArray.Unbind();
	}

	~Cuboid()
	{
		_VertexArray.Destroy();
		_VertexBuffer.Destroy();
	}

	void Render(std::string shader = "" )
	{
		if (shader == "") {
			ShaderManager::Instance()->GetShader(_Shader)->SetVec3("aColor", _Color);
		}
		else {
			ShaderManager::Instance()->GetShader(shader)->SetVec3("aColor", _Color);
			_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(shader)->GetID(), "aPos", 3, VT_FLOAT, 8 * sizeof(float));
			_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(shader)->GetID(), "aNormal", 3, VT_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
			_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(shader)->GetID(), "aTexCoords", 2, VT_FLOAT, 8 * sizeof(float), 6 * sizeof(float));
		}
		_VertexArray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		_VertexArray.Unbind();
	}

    void SetColor(glm::vec3 color) {
        _Color = color;
    }

private:
	glm::vec3 _Color;
};

