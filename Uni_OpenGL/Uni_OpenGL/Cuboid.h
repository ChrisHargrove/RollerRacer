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
		//std::vector<float> vertices;
		//glm::vec3 p1(-((float)Width / 2), -((float)Height / 2), ((float)Depth / 2));
		//glm::vec3 p2(((float)Width / 2), -((float)Height / 2), ((float)Depth / 2));
		//glm::vec3 p3(((float)Width / 2), ((float)Height / 2), ((float)Depth / 2));
		//glm::vec3 p4(-((float)Width / 2), ((float)Height / 2), ((float)Depth / 2));
		//glm::vec3 p5(-((float)Width / 2), -((float)Height / 2), -((float)Depth / 2));
		//glm::vec3 p6(((float)Width / 2), -((float)Height / 2), -((float)Depth / 2));
		//glm::vec3 p7(((float)Width / 2), ((float)Height / 2), -((float)Depth / 2));
		//glm::vec3 p8(-((float)Width / 2), ((float)Height / 2), -((float)Depth / 2));

		//vertices.push_back(p1.x); vertices.push_back(p1.y); vertices.push_back(p1.z);
		//vertices.push_back(p2.x); vertices.push_back(p2.y); vertices.push_back(p2.z);
		//vertices.push_back(p3.x); vertices.push_back(p3.y); vertices.push_back(p3.z);
		//vertices.push_back(p4.x); vertices.push_back(p4.y); vertices.push_back(p4.z);
		//vertices.push_back(p5.x); vertices.push_back(p5.y); vertices.push_back(p5.z);
		//vertices.push_back(p6.x); vertices.push_back(p6.y); vertices.push_back(p6.z);
		//vertices.push_back(p7.x); vertices.push_back(p7.y); vertices.push_back(p7.z);
		//vertices.push_back(p8.x); vertices.push_back(p8.y); vertices.push_back(p8.z);

		//unsigned int indices[] = {
		//	0, 1, 2, 2, 3, 0,// front		
		//	1, 5, 6, 6, 2, 1,// top		
		//	7, 6, 5, 5, 4, 7,// back		
		//	4, 0, 3, 3, 7, 4,// bottom		
		//	4, 5, 1, 1, 0, 4,// left		
		//	3, 2, 6, 6, 7, 3,// right
		//};

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

	void Draw()
	{
		ShaderManager::Instance()->GetShader(_Shader)->SetVec3("aColor", _Color);
		_VertexArray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		_VertexArray.Unbind();
	}

private:
	glm::vec3 _Color;
};
