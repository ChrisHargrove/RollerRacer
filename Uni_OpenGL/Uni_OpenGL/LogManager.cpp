////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "LogManager.h"
#include <Windows.h>

////////////////////////////////////////////////////////////
// Static Variables
////////////////////////////////////////////////////////////
LogManager LogManager::_Instance;

////////////////////////////////////////////////////////////
LogManager::LogManager()
{
	_LogStream = new std::ofstream("OpenGL_Engine_Log.log");
}

////////////////////////////////////////////////////////////
LogManager::~LogManager()
{
	delete _LogStream;
}

////////////////////////////////////////////////////////////
const std::string LogManager::getCurrTime()
{
	std::string result;

	time_t currTime = time(0);

	if (currTime < 0)
		return result;

	//tm* tp = localtime(&currTime);
	tm tp;
	localtime_s(&tp, &currTime);


	const int strTimeLen = 10;
	char strTime[strTimeLen];

	if (strftime(strTime, strTimeLen, "%H:%M:%S", &tp) > 0)
		result.append(strTime);

	return result;
}

////////////////////////////////////////////////////////////
void LogManager::LogInfo(const std::string& Message)
{
#ifdef DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#endif
	LogMessage("[INFO]", Message);
}

////////////////////////////////////////////////////////////
void LogManager::LogDebug(const std::string& Message)
{
#ifdef DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
#endif
	LogMessage("[DEBUG]", Message);
}

////////////////////////////////////////////////////////////
void LogManager::LogWarning(const std::string& Message)
{
#ifdef DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
#endif
	LogMessage("[WARNING]", Message);
}

////////////////////////////////////////////////////////////
void LogManager::LogError(const std::string& Message)
{
#ifdef DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
#endif
	LogMessage("[ERROR]", Message);
}

////////////////////////////////////////////////////////////
void LogManager::LogMessage(const std::string& Tag, const std::string& Message)
{
	(*_LogStream) << '[' << getCurrTime() << ']' << Tag << ' ' << Message << std::endl;
#ifdef DEBUG
	std::cout << '[' << getCurrTime() << ']' << Tag << ' ' << Message << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#endif
}
