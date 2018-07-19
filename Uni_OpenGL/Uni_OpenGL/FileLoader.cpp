#include "FileLoader.h"
#include "LogManager.h"
#include "Tools.h"



FileLoader::FileLoader()
{
	_FileSize = 0;
}


FileLoader::~FileLoader()
{
}

bool FileLoader::OpenInputFile(std::string FilePath)
{
	_FileStream.open(FilePath, std::ios::in);

	if (!_FileStream) {
		LogManager::Instance()->LogError(FilePath + " could not be found!");
		return false;
	}

	//Create empty string to store all data
	std::string data = "";
	if (_FileStream.is_open()) {
		LogManager::Instance()->LogDebug(FilePath + " Opened Successfully!");
		//send file stream pointer to end of file
		_FileStream.seekg(0, std::ios::end);
		//tell data string to reserve memory to the file size
		data.reserve(_FileStream.tellg());
		_FileSize = (int)_FileStream.tellg();
		//send file stream pointer back to beginning
		_FileStream.seekg(0, std::ios::beg);
		//assign all data to the string
		data.assign((std::istreambuf_iterator<char>(_FileStream)), std::istreambuf_iterator<char>());
	}

	//close the file stream as it is now donee with.
	_FileStream.close();

	SplitString(data, "\n", _FileData);

	return true;
}

bool FileLoader::OpenOutputFile(std::string FilePath, int mode)
{
	_FileStream.open(FilePath, mode);
	if (_FileStream.is_open()) {
		LogManager::Instance()->LogDebug(FilePath + " Opened Successfully!");
	}

	return false;
}

void FileLoader::CloseOutputFile()
{
	if (_FileStream.is_open()) {
		LogManager::Instance()->LogDebug("Closing File!");
		_FileStream.close();
	}
}

void FileLoader::WriteToFile(std::string data)
{
	if (_FileStream.is_open()) {
		LogManager::Instance()->LogDebug("Attempting a write to file...");
		_FileStream << data;
	}
}

void FileLoader::WriteToFile(std::vector<std::string> data)
{
	if (_FileStream.is_open()) {
		LogManager::Instance()->LogDebug("Attempting a write to file...");
		for (auto s : data) {
			_FileStream << s;
		}
	}
}

std::vector<std::string> FileLoader::GetData()
{
	return _FileData;
}

int FileLoader::GetFileSize()
{
	return _FileSize;
}
