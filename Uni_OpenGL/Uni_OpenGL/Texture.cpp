#include "Texture.h"
#include <SDL\SDL_image.h>
#include <GLEW\glew.h>
#include "LogManager.h"


std::map<std::string, unsigned int>* Texture::_Textures = new std::map<std::string, unsigned int>;

Texture::Texture()
{
}

Texture::Texture(std::string FileName, std::string textureName)
{
    Load(FileName, textureName);
}


Texture::~Texture()
{
    delete _Textures;
}

bool Texture::SetID(std::string textureName)
{
	auto search = _Textures->find(textureName);
	if (search != _Textures->end()) {
		_ID = search->second;
		return true;
	}
	else {
		LogManager::Instance()->LogError("Texture does not exist: " + textureName);
		return false;
	}
	return false;
}

bool Texture::Load(std::string FileName, std::string textureName)
{
    LogManager::Instance()->LogWarning("Trying to load image " + FileName);
	auto search = _Textures->find(textureName);
	if (search == _Textures->end()) {
		SDL_Surface* image;
		//check to see if image was found
		if ((image = IMG_Load(FileName.c_str()))) {
			//check to see if image was loaded
			if (!image) {
				LogManager::Instance()->LogError("Didnt load image " + FileName);
				return false;
			}
			//save image size just in case
			_Size = glm::vec2(image->w, image->h);

			//This bit makes using image formats with transparency possible
			GLenum format;
			if (image->format->Amask > 0) {
				format = GL_RGBA;
			}
			else {
				format = GL_RGB;
			}

			//create opengl texture
			glGenTextures(1, &_ID);
			//bind texture
			glBindTexture(GL_TEXTURE_2D, _ID);
			//set texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//Linear Filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//Generate The Texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
			//generate a mipmap
			glGenerateMipmap(GL_TEXTURE_2D);

			if (image) { //free any memory that may have been used.
				SDL_FreeSurface(image);
			}

			_Textures->emplace(std::pair<std::string, unsigned int>(textureName, _ID));
			LogManager::Instance()->LogInfo("Texture " + FileName + " Loaded...");

			return true;
        }
        else {
            LogManager::Instance()->LogError("Texture " + FileName + " Failed...");
        }
    }
	else {
		LogManager::Instance()->LogError("Texture Name already taken, " + FileName + " Not Loaded!");
		return false;
	}
    return false;
}

bool Texture::Unload(std::string textureName)
{
	auto search = _Textures->find(textureName);
	if (search != _Textures->end()) {
		glDeleteTextures(1, &search->second);
		_Textures->erase(textureName);
		LogManager::Instance()->LogInfo("Unloaded " + textureName);
		return true;
	}
	else {
		LogManager::Instance()->LogError("Texture Does not exist, can not remove!");
		return false;
	}
}

void Texture::UnloadAll()
{
	LogManager::Instance()->LogInfo("Removing all textures...");
	for (auto t : *_Textures) {
		glDeleteTextures(1, &t.second);
		_Textures->erase(t.first);
	}
	LogManager::Instance()->LogInfo("All textures unloaded!");
}
