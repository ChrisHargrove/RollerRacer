#pragma once

#include "Buffer.h"
#include <GLM\glm.hpp>


class PrimitiveShape
{
public:
	PrimitiveShape() {};

	virtual ~PrimitiveShape() {};

	virtual void Render() = 0;

protected:
	int _DrawCount;
	std::string _Shader;

	Buffer _VertexArray;
	Buffer _VertexBuffer;
	
};

