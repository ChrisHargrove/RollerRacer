#pragma once

#include "CubeMap.h"
#include "PrimitiveShape.h"
#include "ShaderManager.h"

class SkyBox : public PrimitiveShape
{
public:
	SkyBox(std::string ShaderName, std::string FileName, std::string cubeMapName);
	~SkyBox();

    void Draw();

private:
    
    CubeMap* _CubeMapTexture;
};

