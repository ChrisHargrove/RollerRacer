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
    ~Texture();

    inline unsigned int GetID() { return _ID; }
    inline glm::vec2 GetSize() { return _Size; }

	bool SetID(std::string textureName);

    bool Load(std::string FileName, std::string textureName);
	bool Unload(std::string textureName);
	void UnloadAll();

private:
    unsigned int _ID;
    glm::vec2 _Size;
	
	static std::map<std::string, unsigned int>* _Textures;
};

