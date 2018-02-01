#pragma once

#include <SDL\SDL.h>
#include <GLM\glm.hpp>
#include <string>

class Texture
{
public:
    Texture();
    Texture(std::string FileName);
    ~Texture();

    inline unsigned int GetID() { return _ID; }
    inline glm::vec2 GetSize() { return _Size; }

    bool Load(std::string FileName);

private:
    unsigned int _ID;
    glm::vec2 _Size;
};

