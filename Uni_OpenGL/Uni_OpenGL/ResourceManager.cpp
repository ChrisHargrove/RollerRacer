#include "ResourceManager.h"
#include "FileLoader.h"
#include "Tools.h"

ResourceManager ResourceManager::_Instance;

bool ResourceManager::LoadTexture(std::string FilePath, std::string Key)
{
	auto search = _TextureList.find(Key);
	if (search == _TextureList.end()) {
		_TextureList.emplace(std::make_pair(Key, new Texture(FilePath, Key)));
		return true;
	}
	return false;
}

Texture * ResourceManager::GetTexture(std::string Key)
{
	auto search = _TextureList.find(Key);
	if (search != _TextureList.end()) {
		return _TextureList.at(Key);
	}
	return nullptr;
}

Texture * ResourceManager::LoadUseTexture(std::string FilePath, std::string Key)
{
	auto search = _TextureList.find(Key);
	if (search != _TextureList.end()) {
		return _TextureList.at(Key);
	}
	else {
		_TextureList.emplace(std::make_pair(Key, new Texture(FilePath, Key)));
		return _TextureList.at(Key);
	}
}

bool ResourceManager::LoadSkybox(std::string FilePath, std::string Key)
{
	auto search = _SkyBoxList.find(Key);
	if (search == _SkyBoxList.end()) {
		_SkyBoxList.emplace(std::make_pair(Key, new SkyBox("skybox", FilePath, Key)));
		return true;
	}
	return false;
}

SkyBox * ResourceManager::GetSkyBox(std::string Key)
{
	auto search = _SkyBoxList.find(Key);
	if (search != _SkyBoxList.end()) {
		return _SkyBoxList.at(Key);
	}
	return nullptr;
}

SkyBox * ResourceManager::LoadUseSkyBox(std::string FilePath, std::string Key)
{
	auto search = _SkyBoxList.find(Key);
	if (search != _SkyBoxList.end()) {
		return _SkyBoxList.at(Key);
	}
	else {
		_SkyBoxList.emplace(std::make_pair(Key, new SkyBox("skybox", FilePath, Key)));
		return _SkyBoxList.at(Key);
	}
}

bool ResourceManager::LoadTextRenderer(std::string FilePath, std::string Key)
{
	auto search = _TextRendererList.find(Key);
	if (search == _TextRendererList.end()) {
		_TextRendererList.emplace(std::make_pair(Key, new TextRenderer(FilePath)));
		return true;
	}
	return false;
}

TextRenderer * ResourceManager::GetTextRenderer(std::string Key)
{
	auto search = _TextRendererList.find(Key);
	if (search != _TextRendererList.end()) {
		return _TextRendererList.at(Key);
	}
	return nullptr;
}

TextRenderer * ResourceManager::LoadUseTextRenderer(std::string FilePath, std::string Key)
{
	auto search = _TextRendererList.find(Key);
	if (search != _TextRendererList.end()) {
		return _TextRendererList.at(Key);
	}
	else {
		_TextRendererList.emplace(std::make_pair(Key, new TextRenderer(FilePath)));
		return _TextRendererList.at(Key);
	}
}

bool ResourceManager::LoadModel(std::string FilePath, std::string Shader, std::string Key)
{
	auto search = _ModelList.find(Key);
	if (search == _ModelList.end()) {
		_ModelList.emplace(std::make_pair(Key, new Model(FilePath, Shader)));
		return true;
	}
	return false;
}

Model * ResourceManager::GetModel(std::string Key)
{
	auto search = _ModelList.find(Key);
	if (search != _ModelList.end()) {
		return _ModelList.at(Key);
	}
	return nullptr;
}

Model * ResourceManager::LoadUseModel(std::string FilePath, std::string Shader, std::string Key)
{
	auto search = _ModelList.find(Key);
	if (search != _ModelList.end()) {
		return _ModelList.at(Key);
	}
	else {
		_ModelList.emplace(std::make_pair(Key, new Model(FilePath, Shader)));
		return _ModelList.at(Key);
	}
}

bool ResourceManager::LoadHighScores(std::string FilePath)
{
	FileLoader _Loader;
	if (!_Loader.OpenInputFile(FilePath)) {
		return false;
	}
	std::vector<std::string> _HighscoreData = _Loader.GetData();
	for (int i = 0; i < _HighscoreData.size(); i += 3) {
		if (_HighscoreData[i] == "") continue;
		int _LevelSeed = stoi(SplitEnd(_HighscoreData[i], ":"));
		HighScore _Score;
		_Score._LapTime = std::stof(SplitEnd(_HighscoreData[i + 1], ":"));
		_Score._Initials = SplitEnd(_HighscoreData[i + 2], ":");
		_HighScoreList.emplace(std::make_pair(_LevelSeed, _Score));
	}
	return true;
}

bool ResourceManager::SaveHighScores(std::string FilePath)
{
	FileLoader _Loader;
	if (_Loader.OpenOutputFile(FilePath)) {
		return false;
	}
	std::vector<std::string> DataToWrite;
	for (auto s : _HighScoreList) {
		DataToWrite.push_back("Level:" + std::to_string(s.first) + "\n");
		DataToWrite.push_back("LapTime:" + std::to_string(s.second._LapTime) + "\n");
		DataToWrite.push_back("Initials:" + s.second._Initials + "\n");
	}
	_Loader.WriteToFile(DataToWrite);

	return true;
}

std::map<int, HighScore>* ResourceManager::GetHighScores()
{
	return &_HighScoreList;
}

bool ResourceManager::LoadOptions(std::string FilePath)
{
	FileLoader _Loader;
	if (!_Loader.OpenInputFile(FilePath)) {
		return false;
	}
	std::vector<std::string> _Options = _Loader.GetData();
	for (int i = 0; i < _Options.size(); i++) {
		if (_Options[i] == "") continue;
		_OptionData.emplace(std::make_pair(SplitFront(_Options[i], ":"), stoi(SplitEnd(_Options[i], ":"))));
	}
	return true;
}

bool ResourceManager::SaveOptions(std::string FilePath)
{
	FileLoader _Loader;
	if (_Loader.OpenOutputFile(FilePath)) {
		return false;
	}
	std::vector<std::string> DataToWrite;
	for (auto o : _OptionData) {
		DataToWrite.push_back(o.first + ":" + std::to_string(o.second) + "\n");
	}
	_Loader.WriteToFile(DataToWrite);
}

std::map<std::string, int>* ResourceManager::GetOptions()
{
	return &_OptionData;
}

bool ResourceManager::LoadCamera(glm::vec3 position)
{
	_GameCamera = new Camera(position);
	return true;
}

Camera * ResourceManager::GetCamera()
{
	return _GameCamera;
}

void ResourceManager::UnloadTextures()
{
	for (auto t : _TextureList) {
		delete t.second;
	}
}

void ResourceManager::UnloadSkyBoxes()
{
	for (auto s : _SkyBoxList) {
		delete s.second;
	}
}

void ResourceManager::UnloadCamera()
{
	delete _GameCamera;
}

void ResourceManager::UnloadTextRenderers()
{
	for (auto tr : _TextRendererList) {
		delete tr.second;
	}
}

void ResourceManager::UnloadModels()
{
	for (auto m : _ModelList) {
		delete m.second;
	}
}

void ResourceManager::UnloadAllResources()
{
	UnloadCamera();
	UnloadModels();
	UnloadSkyBoxes();
	UnloadTextRenderers();
	UnloadTextures();
}

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}
