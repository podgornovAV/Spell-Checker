#pragma once
#include <list>
#include "DictionaryType.h"
#define DELEMITER "===" 
 
class ReadData
{
public:	
	static void readInput(dictionary_t& dic, std::list<std::string>& text);
private:
	ReadData();
	static void readDictionary(dictionary_t& dic);
	static void readText(std::list<std::string>& text);
	static void lineToDictionary(const std::string& line, dictionary_t& dictionary);
};

