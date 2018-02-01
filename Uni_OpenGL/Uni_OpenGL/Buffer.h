#ifndef BUFFER_H
#define BUFFER_H

#include <GLEW\glew.h>
#include <string>

enum BufferType {
	VAO,
	VBO,
	EBO
};

enum DrawType {
	STATIC = GL_STATIC_DRAW,
	DYNAMIC = GL_DYNAMIC_DRAW,
};

enum VariableType {
	VT_BYTE = GL_BYTE,
	VT_U_BYTE = GL_UNSIGNED_BYTE,
	VT_SHORT = GL_SHORT,
	VT_U_SHORT = GL_UNSIGNED_SHORT,
	VT_FLOAT = GL_FLOAT
};

class Buffer
{
public:
	Buffer();
	~Buffer();

	void Create(BufferType Type);
	void Bind();
	void Unbind();
	void Fill(int DataSize, const void* Data, DrawType Type);
	void Destroy();

	void AddAttribPointer(unsigned int ShaderID, const std::string & name, int size, VariableType Type, int stride = 0, int offset = 0);

	unsigned int GetID() const;

private:
	unsigned int _ID;
	BufferType _Type;
};

#endif