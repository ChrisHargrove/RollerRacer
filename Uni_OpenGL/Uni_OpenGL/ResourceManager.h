#pragma once

#include <map>
#include <string>
#include "Texture.h"
#include "SkyBox.h"
#include "TextRenderer.h"
#include "Camera.h"
#include "Model.h"
#include "HighScoreState.h"

class ResourceManager
{
public:
	bool LoadTexture(std::string FilePath, std::string Key);
	Texture* GetTexture(std::string Key);
	Texture* LoadUseTexture(std::string FilePath, std::string Key);

	bool LoadSkybox(std::string FilePath, std::string Key);
	SkyBox* GetSkyBox(std::string Key);
	SkyBox* LoadUseSkyBox(std::string FilePath, std::string Key);

	bool LoadTextRenderer(std::string FilePath, std::string Key);
	TextRenderer* GetTextRenderer(std::string Key);
	TextRenderer* LoadUseTextRenderer(std::string FilePath, std::string Key);

	bool LoadModel(std::string FilePath, std::string Shader, std::string Key);
	Model* GetModel(std::string Key);
	Model* LoadUseModel(std::string FilePath, std::string Shader, std::string Key);

	bool LoadHighScores(std::string FilePath);
	bool SaveHighScores(std::string FilePath);
	std::map<int, HighScore>* GetHighScores();

	bool LoadOptions(std::string FilePath);
	bool SaveOptions(std::string FilePath);
	std::map<std::string, int>* GetOptions();


	bool LoadCamera(glm::vec3 position);
	Camera* GetCamera();

	void UnloadTextures();
	void UnloadSkyBoxes();
	void UnloadCamera();
	void UnloadTextRenderers();
	void UnloadModels();
	void UnloadAllResources();

	static ResourceManager* Instance() {
		return &_Instance;
	}

private:
	static ResourceManager _Instance;

	std::map<std::string, Texture*> _TextureList;
	std::map<std::string, SkyBox*> _SkyBoxList;
	std::map<std::string, TextRenderer*> _TextRendererList;
	std::map<std::string, Model*> _ModelList;
	std::map<int, HighScore> _HighScoreList;
	std::map<std::string, int> _OptionData;

	Camera* _GameCamera;

	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager&) {}
};

