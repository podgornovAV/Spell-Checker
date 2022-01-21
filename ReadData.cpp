#include "ReadData.h"
#include <sstream>
#include <iostream>

using namespace std;

void ReadData::readInput(dictionary_t& dic, std::list<std::string>& text)
{
	readDictionary(dic);
	readText(text);
}

void ReadData::readDictionary(dictionary_t& dic)
{
	string inputLine;
	getline(cin, inputLine);

	while (inputLine != DELEMITER)
	{
		lineToDictionary(inputLine, dic);
		getline(cin, inputLine);

	}
}

void ReadData::readText(std::list<std::string>& text)
{
	string inputLine;
	getline(cin, inputLine);
	while (inputLine != DELEMITER)
	{
		text.push_back(inputLine);
		getline(cin, inputLine);
	}
}

void ReadData::lineToDictionary(const std::string& line, dictionary_t& dictionary)
{
	static uint64_t index = 0;
	if (line != DELEMITER)
	{
		stringstream ss(line);
		string word;
		while (ss >> word) {
			dictionary[word.length()].insert(make_pair(word, index++));
		}
	}
}
