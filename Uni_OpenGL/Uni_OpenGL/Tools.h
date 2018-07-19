#pragma once

#include <string>
#include <iostream>
#include <vector>

//#################################################################################
//	Returns a copy of a subsection of a string.
//	--string-- String to chop a piece from.
//	--start-- Starting position of where to chop.
//	--length-- How many characters to chop out.
//#################################################################################
std::string ChopStringStart(std::string string, unsigned int start, unsigned int length);

//#################################################################################
//	Modifies a string to chopped piece.
//	--string-- String to chop a piece from.
//	--start-- Starting position of where to chop.
//	--length-- How many characters to chop out.
//#################################################################################
void ChopStringStart(std::string& string, unsigned int start, unsigned int length);

std::string SplitEnd(const std::string& string, const std::string& delimeter);
std::string SplitFront(const std::string& string, const std::string& delimeter);

std::string FloatToTime(float time);

//#################################################################################
//	Adds two strings together and returns a copy of the new string.
//	--stringA-- The first string that your adding to.
//	--stringB-- The second string that your adding.
//#################################################################################
std::string AddString(std::string stringA, std::string stringB);

//#################################################################################
//	Modifies a string to add another to it.
//	--stringA-- The first string that your adding to.
//	--stringB-- The second string that your adding.
//#################################################################################
void AddString(std::string& stringA, const std::string& stringB);

//#################################################################################
//	Displays a string to the screen with each character on a seperate line. And then
//	shows the string size at the end;
//	--string-- The  string that youwish to display.
//	--mode-- Which loop do you want to use?
//#################################################################################
void ListString(const std::string& string, bool mode = true);

//=================================================================================
// ACTUALLY USEFUL FUNCTIONS
//=================================================================================

//#################################################################################
//	Will split a string and split into pieces using a delimeter and store them into
//	a container.
//	--string-- The string that you wish to split.
//	--delimeter-- Which character to split the string at.
//	--store-- Reference to a vector to store the split strings into.
//#################################################################################
void SplitString(const std::string& string, const std::string& delimeter, std::vector<std::string>& store);

//#################################################################################
//	Will get the string between two delimeters from another string.
//	--string-- The string that you wish to split.
//	--delimeter-- Which character to split the string at.
//	--delimeterTwo-- Which character to split the string to.
//	--store-- Reference to a vector to store the split strings into.
//#################################################################################
bool GetStringBetween(const std::string& string, const std::string& delimeter, const std::string& delimeterTwo, std::string& store);

//#################################################################################
//	Will return a value from a string using a delimeter to find the value.
//	--string-- The string that you wish to split.
//	--varName-- The variable your looking for.
//	--store-- Reference to variable to store the data.
//#################################################################################
bool FloatFromString(const std::string& string, const std::string& varName, float& store);

//#################################################################################
//	Will return two values from a string using a delimeter to find the value.
//	--string-- The string that you wish to split.
//	--varName-- The variable your looking for.
//	--store-- Reference to variable to store the data.
//	--storeTwo-- Reference to variable to store the data.
//#################################################################################
bool Float2FromString(const std::string& string, const std::string& varName, float& store, float& storeTwo);

//#################################################################################
//	Will return a value from a string using a delimeter to find the value.
//	--string-- The string that you wish to split.
//	--varName-- The variable your looking for.
//	--store-- Reference to variable to store the data.
//#################################################################################
bool StringFromString(const std::string& string, const std::string& varName, std::string& store);
