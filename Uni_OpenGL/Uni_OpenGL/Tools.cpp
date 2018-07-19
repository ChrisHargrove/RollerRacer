#include "Tools.h"

std::string ChopStringStart(std::string string, unsigned int start, unsigned int length)
{
	return string.substr(start, length);
}

void ChopStringStart(std::string & string, unsigned int start, unsigned int length)
{
	string = string.substr(start, length);
}

std::string SplitEnd(const std::string & string, const std::string & delimeter)
{
	unsigned int position = 0;
	position = string.find(delimeter) + 1;
	return string.substr(position, string.length() - position);
}

std::string SplitFront(const std::string & string, const std::string & delimeter)
{
	unsigned int position = 0;
	position = string.find(delimeter);
	return string.substr(0, position);
}

std::string FloatToTime(float time)
{
	int minutes = time / 60;
	float milliseconds = time - (int)time;
	int seconds = (int)time % 60;
	std::string laptimestring = std::to_string(minutes) + ":" + std::to_string(seconds) + ":" + std::to_string(milliseconds).substr(2, 2);
	return laptimestring;
}

std::string AddString(std::string stringA, std::string stringB)
{
	return stringA.append(stringB);
}

void AddString(std::string & stringA, const std::string & stringB)
{
	stringA = stringA.append(stringB);
}

void ListString(const std::string & string, bool mode)
{
	if (mode) {
		for (auto c : string) {
			std::cout << c << std::endl;
		}
		std::cout << "String Size: " << string.size() << std::endl;
	}
	else {
		for (auto it = string.begin(); it != string.end(); it++) {
			std::cout << *it << std::endl;
		}
		std::cout << "String Size: " << string.size() << std::endl;
	}
}

void SplitString(const std::string & string, const std::string & token, std::vector<std::string>& store)
{
	unsigned int position = 0;
	unsigned int prevPosition = 0;
	while ((position = string.find(token, prevPosition)) != std::string::npos) {
		store.push_back(string.substr(prevPosition, position - prevPosition));
		prevPosition = position + 1;
	}
	store.push_back(string.substr(prevPosition));
}

bool GetStringBetween(const std::string & string, const std::string & delimeter, const std::string & delimeterTwo, std::string & store)
{
	unsigned int start, end;
	start = end = 0;

	if ((start = string.find(delimeter) + delimeter.size()) != std::string::npos 
	&& (end = string.find(delimeterTwo, string.find(delimeter))) != std::string::npos) {
		store = string.substr(start, end - start);
	}
	return false;
}

bool FloatFromString(const std::string & string, const std::string & varName, float& store)
{
	unsigned int position = 0;
	if ((position = string.find(varName)) != std::string::npos) {
		store = std::stof(string.substr(string.find("=", position) + 1));
		return true;
	}
	return false;
}

bool Float2FromString(const std::string & string, const std::string & varName, float & store, float & storeTwo)
{
	unsigned int position = 0;
	if ((position = string.find(varName)) != std::string::npos) {
		std::string temp = string.substr(string.find("=", position) + 1);
		store = std::stof(temp.substr(0, temp.find(' ')));
		storeTwo = std::stof(temp.substr(temp.find(' ') + 1));
		return true;
	}
	return false;
}

bool StringFromString(const std::string & string, const std::string & varName, std::string & store)
{
	unsigned int position = 0;
	if ((position = string.find(varName)) != std::string::npos) {
		store = string.substr(string.find("=", position) + 1);
		return true;
	}
	return false;
}
