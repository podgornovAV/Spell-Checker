#include <iostream>
#include <chrono>
#include "SpellChecker.h"
#include "ReadData.h"
using namespace std;

int main()
{
	dictionary_t dictionary;
	list<string> text;

	ReadData::readInput(dictionary, text);

	SpellChecker spellChecker(dictionary);
	spellChecker.makeTextCorrection(text);
	
	return 0;
}


