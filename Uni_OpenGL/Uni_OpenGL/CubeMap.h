#pragma once

#include <string>
#include <vector>
#include <map>

class CubeMap
{
public:
    CubeMap();
    CubeMap(std::string FileName, std::string cubeMapName);
    ~CubeMap();

	unsigned int GetID() { return _ID; }

    bool Load(std::string FileName, std::string cubeMapName);

private:
    unsigned int _ID;

    std::vector<std::string> _FaceNames;
};

