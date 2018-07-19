#pragma once

#include <SDL\SDL.h>
#include <GLM\glm.hpp>
#include <string>
#include <map>

class Texture
{
public:
    Texture();
    Texture(std::string FileName, std::string textureName);
    virtual ~Texture();

    unsigned int GetID() { return _ID; }
	void SetID(unsigned int id) { _ID = id; }
    glm::vec2 GetSize() { return _Size; }

    bool Load(std::string FileName, std::string textureName);
	bool Unload();

private:
    unsigned int _ID;
    glm::vec2 _Size;
};

