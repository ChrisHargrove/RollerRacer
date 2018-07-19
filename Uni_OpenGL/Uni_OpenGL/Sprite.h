#pragma once

#include <GLM\glm.hpp>
#include <string>
#include <vector>
#include "PrimitiveShape.h"
#include "ShaderManager.h"
#include "Texture.h"

class Sprite : public PrimitiveShape
{
public:
	Sprite(glm::vec2 size, int rows, int cols, Texture* texture, std::string ShaderName, glm::vec3 Color = glm::vec3(1.0, 1.0, 1.0)) {
		_Shader = ShaderName;
		_ImageSize = glm::vec2(rows, cols);
		_SpriteTexture = texture;
		_Size = size;


		float vertices[] = {
			0.0f,0.0f,0.0f,						0.0f,  0.0f, -1.0f,
			0.0f,1.0f * _Size.y,0.0f,			0.0f,  0.0f, -1.0f,
			1.0f * _Size.x,1.0f * _Size.y,0.0f,	0.0f,  0.0f, -1.0f,
			1.0f * _Size.x,0.0f,0.0f,				0.0f,  0.0f, -1.0f,
		};

		uvs.reserve(8);
		uvs.push_back(0.0f); uvs.push_back(0.0f);
		uvs.push_back(0.0f); uvs.push_back(1.0f);
		uvs.push_back(1.0f); uvs.push_back(1.0f);
		uvs.push_back(1.0f); uvs.push_back(0.0f);

		unsigned int indices[] = {
			0, 1, 2,
			3, 0, 2,
		};

		_VertexArray.Create(VAO);
		_VertexBuffer.Create(VBO);
		_UVBuffer.Create(VBO);
		_IndexBuffer.Create(EBO);

		_VertexArray.Bind();

		_VertexBuffer.Fill(sizeof(vertices), vertices, STATIC);
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aPos", 3, VT_FLOAT, 6 * sizeof(float));
		_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aNormal", 3, VT_FLOAT, 6 * sizeof(float), 3 * sizeof(float));

		_UVBuffer.Fill(sizeof(float) * uvs.size(), &uvs[0], DYNAMIC);
		_UVBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aTexCoords", 2, VT_FLOAT, 2 * sizeof(float));

		_IndexBuffer.Fill(sizeof(indices), indices, STATIC);

		_Color = Color;
		_DrawCount = 6;
		_VertexArray.Unbind();

	}

	~Sprite() 
	{
		_VertexArray.Destroy();
		_VertexBuffer.Destroy();
	}

	void Render(std::string shader = "")
	{
		ShaderManager::Instance()->GetShader(_Shader)->SetInt("textureImage", 0);
		_VertexArray.Bind();
		if (_SpriteTexture != nullptr) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _SpriteTexture->GetID());
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		_VertexArray.Unbind();
	}

	void SetSpriteSheetLocation(float x = 0.0f, float y = 0.0f)
	{
		uvs.clear();
		float stepX = 1.0f / _ImageSize.x;
		float stepY = 1.0f / _ImageSize.y;
		uvs.push_back(x * stepX);			uvs.push_back((y * stepY) + stepY);
		uvs.push_back(x * stepX);			uvs.push_back(y * stepY);
		uvs.push_back((x * stepX) + stepX); uvs.push_back(y * stepY);
		uvs.push_back((x * stepX) + stepX); uvs.push_back((y * stepY) + stepY);

		_UVBuffer.Fill(sizeof(float) * uvs.size(), &uvs[0], DYNAMIC);
	}

	glm::vec2 GetSize() { return _Size; }

private:
	glm::vec3 _Color;
	Buffer	_IndexBuffer;
	Buffer  _UVBuffer;

	Texture* _SpriteTexture;

	std::vector<float> uvs;
	glm::vec2 _ImageSize;
	glm::vec2 _Size;
};

