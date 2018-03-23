#include "CubeMap.h"
#include "LogManager.h"
#include <SDL\SDL_image.h>
#include <GLEW\glew.h>

std::map<std::string, unsigned int>* CubeMap::_CubeMaps = new std::map<std::string, unsigned int>;

CubeMap::CubeMap()
{
}

CubeMap::CubeMap(std::string FileName, std::string cubeMapName)
{
    _FaceNames.reserve(6);
    _FaceNames.push_back("Right");
    _FaceNames.push_back("Left");
    _FaceNames.push_back("Top");
    _FaceNames.push_back("Bottom");
    _FaceNames.push_back("Front");
    _FaceNames.push_back("Back");
    Load(FileName, cubeMapName);
}


CubeMap::~CubeMap()
{
    delete _CubeMaps;
}

bool CubeMap::SetID(std::string cubeMapName)
{
    auto search = _CubeMaps->find(cubeMapName);
    if (search != _CubeMaps->end()) {
        _ID = search->second;
        return true;
    }
    else {
        LogManager::Instance()->LogError("CubeMap does not exist: " + cubeMapName);
        return false;
    }
    return false;
}

bool CubeMap::Load(std::string FileName, std::string cubeMapName)
{
    auto search = _CubeMaps->find(cubeMapName);
    if (search == _CubeMaps->end()) {

        //create the texture
        glGenTextures(1, &_ID);
        //bind texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);

        SDL_Surface* image;
        for (int i = 0; i < (int)_FaceNames.size(); i++) {
            if (image = IMG_Load((FileName + "_" + _FaceNames[i] + ".jpg").c_str())) {
                //check to see if image loaded
                if (!image) {
                    LogManager::Instance()->LogError("Didnt load image " + (FileName + "_" + _FaceNames[i]));
                    return false;
                }
                //sorts format for transparency if needed
                GLenum format;
                if (image->format->Amask > 0) {
                    format = GL_RGBA;
                }
                else {
                    format = GL_RGB;
                }

                //Generate texture data
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

            }
            if (image) {
                SDL_FreeSurface(image);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        _CubeMaps->emplace(std::pair<std::string, unsigned int>(cubeMapName, _ID));
        LogManager::Instance()->LogInfo("CubeMap " + FileName + " Loaded...");

        return true;
    }
    else {
        LogManager::Instance()->LogError("CubeMap Name already taken, " + FileName + " Not Loaded!");
        return false;
    }
}

bool CubeMap::Unload(std::string cubeMapName)
{
    auto search = _CubeMaps->find(cubeMapName);
    if (search != _CubeMaps->end()) {
        glDeleteTextures(1, &search->second);
        _CubeMaps->erase(cubeMapName);
        LogManager::Instance()->LogInfo("Unloaded " + cubeMapName);
        return true;
    }
    LogManager::Instance()->LogError("CubeMap Does not exist, can not remove!");
    return false;
}

void CubeMap::UnloadAll()
{
    LogManager::Instance()->LogInfo("Removing all CubeMaps...");
    for (auto c : *_CubeMaps) {
        glDeleteTextures(1, &c.second);
        _CubeMaps->erase(c.first);
    }
    LogManager::Instance()->LogInfo("All CubeMaps unloaded!");
}
