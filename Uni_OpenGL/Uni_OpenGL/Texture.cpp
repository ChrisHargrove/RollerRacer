#include "Texture.h"
#include <SDL\SDL_image.h>
#include <GLEW\glew.h>
#include "LogManager.h"


Texture::Texture()
{
}

Texture::Texture(std::string FileName)
{
    Load(FileName);
}


Texture::~Texture()
{
}

bool Texture::Load(std::string FileName)
{
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE,image->pixels);
        //generate a mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        if (image) { //free any memory that may have been used.
            SDL_FreeSurface(image);
        }

        return true;
    }
    return false;
}
