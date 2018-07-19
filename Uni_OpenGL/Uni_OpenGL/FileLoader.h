#pragma once

#include <fstream>
#include <string>
#include <vector>

class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	bool OpenInputFile(std::string FilePath);
	bool OpenOutputFile(std::string FilePath, int mode = std::ios::out);
	void CloseOutputFile();

	void WriteToFile(std::string data);
	void WriteToFile(std::vector<std::string> data);

	std::vector<std::string> GetData();
	int GetFileSize();

private:
	std::fstream _FileStream;

	int _FileSize;
	std::vector<std::string> _FileData;
};

