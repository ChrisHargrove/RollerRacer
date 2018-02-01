#include "Buffer.h"
#include "LogManager.h"

Buffer::Buffer() : _ID(0)
{
}


Buffer::~Buffer()
{
}

void Buffer::Create(BufferType Type)
{
	if (_ID == 0) { //Stops re-genning a buffer
		if (Type == VAO) {
			glGenVertexArrays(1, &_ID);
			_Type = Type;
		}
		else {
			glGenBuffers(1, &_ID);
			_Type = Type;
		}
	}
}

void Buffer::Bind()
{
	if (_Type == VAO) {
		glBindVertexArray(_ID);
	}
	else if (_Type == EBO){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ID);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, _ID);
	}
}

void Buffer::Unbind()
{
	if (_Type == VAO) {
		glBindVertexArray(0);
	}
	else if (_Type == EBO) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Buffer::Fill(int DataSize, const void * Data, DrawType Type)
{
	
	if (_Type != VAO) {
		Bind();
		if (_Type == EBO) {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, DataSize, Data, Type);
		}
		else {
			glBufferData(GL_ARRAY_BUFFER, DataSize, Data, Type);
		}
	}
	else {
		LogManager::Instance()->LogWarning("You cant fill a Vertex Array Object With Data!...");
	}
}

void Buffer::Destroy()
{
	if (_Type == VAO) {
		glDeleteVertexArrays(1, &_ID);
	}
	else {
		glDeleteBuffers(1, &_ID);
	}
}

void Buffer::AddAttribPointer(unsigned int ShaderID, const std::string & name, int size, VariableType Type, int stride, int offset) 
{
	if (_Type != VAO) {
		GLuint AttribLoc = glGetAttribLocation(ShaderID, name.c_str());
		glVertexAttribPointer(AttribLoc, size, Type, GL_FALSE, stride,(void*) offset);
		glEnableVertexAttribArray(AttribLoc);
	}
	else {
		LogManager::Instance()->LogWarning("You cant assign an attribute pointer to a Vertex Array Object!...");
	}
}

unsigned int Buffer::GetID() const
{
	return _ID;
}
