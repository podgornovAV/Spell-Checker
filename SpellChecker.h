#pragma once
#include <iostream>
#include <list>
#include "DictionaryType.h"


class SpellChecker
{
public:
	SpellChecker(dictionary_t& _dictionary) :dictionary(_dictionary) {};
	void makeTextCorrection(const std::list<std::string>& text);
	
private:
	dictionary_t& dictionary;

	std::unordered_map<std::string, std::string> bufferWordCorrections;
	std::map<uint64_t, std::string> wordCorrections;

	void makeLineCorrection(const std::string& textLine, std::ostream& output);
	void checkWord(const std::string& word, std::ostream& correctionOut);

	bool checkCorectionsWhitOneEdits(const std::string& word);
	bool checkCorectionsWhitTwoEdits(const std::string& word);

	bool checkEqualLen(const std::string& word);

	bool checkLenLessByOne(const std::string& word);
	bool checkLenMoreByOne(const std::string& word);

	bool checkLenLessByTwo(const std::string& word);
	bool checkLenMoreByTwo(const std::string& word);

	bool InsensitiveCaseCompareChar(char a, char b);
	bool InsensitiveCaseCompareChar(char a, char b, char c);
};

