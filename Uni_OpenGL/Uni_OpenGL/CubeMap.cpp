#include "CubeMap.h"
#include "LogManager.h"
#include <SDL\SDL_image.h>
#include <GLEW\glew.h>

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
	glDeleteTextures(1, &_ID);
}

bool CubeMap::Load(std::string FileName, std::string cubeMapName)
{
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
    LogManager::Instance()->LogInfo("CubeMap " + FileName + " Loaded...");

    return true;

}
