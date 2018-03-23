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

    inline unsigned int GetID() { return _ID; }

    bool SetID(std::string cubeMapName);

    bool Load(std::string FileName, std::string cubeMapName);
    bool Unload(std::string cubeMapName);
    void UnloadAll();

private:
    unsigned int _ID;

    std::vector<std::string> _FaceNames;

    static std::map<std::string, unsigned int>* _CubeMaps;
};

