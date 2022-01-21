#include "SpellChecker.h"


using namespace std;

bool SpellChecker::InsensitiveCaseCompareChar(char a, char b)
{
	return tolower(a) == tolower(b);
}
inline bool SpellChecker::InsensitiveCaseCompareChar(char a, char b, char c)
{
	return (tolower(a) == tolower(b)) && (tolower(a) == tolower(c)) && (tolower(c) == tolower(b));
}

void SpellChecker::makeTextCorrection(const std::list<std::string>& text)
{
	bufferWordCorrections.clear();
	for (auto& line : text)
	{
		makeLineCorrection(line, cout);
		cout << endl;
	}
}

void SpellChecker::makeLineCorrection(const std::string& textLine, std::ostream& correctionOut)
{
	string delimiter = " ";
	size_t pos = 0;
	string word = "";
	bool newWord = true;
	for (int i = 0; i < textLine.length(); i++)
	{
		if ((textLine[i] == ' '))
		{
			if (i != 0)
			{
				if (textLine[i - 1] != ' ')
				{
					checkWord(word, correctionOut);
					word.clear();
				}
			}
			correctionOut << textLine[i];
		}
		else
		{
			word.push_back(textLine[i]);
			if (i == (textLine.length() - 1))
				checkWord(word, correctionOut);

		}
	}
}
void SpellChecker::checkWord(const std::string& word, std::ostream& correctionOut)
{
	if (!word.empty())
	{
		string corection;
		wordCorrections.clear();
		map<unsigned, string> answers;
		auto pairBuf = bufferWordCorrections.find(word);
		if (pairBuf != bufferWordCorrections.end())
		{
			correctionOut << (*pairBuf).second;
		}
		else
		{
			auto wordPair = dictionary[word.length()].find(word);
			if (wordPair != dictionary[word.length()].end())
			{
				corection = word;
				correctionOut << word;
			}
			else
			{
				if (!checkCorectionsWhitOneEdits(word))
				{
					if (!checkCorectionsWhitTwoEdits(word))
					{
						corection = "{" + word + "?}";
						correctionOut << corection;
					}
				}
				if (wordCorrections.size() > 1)
				{
					corection += "{";
					for (auto& W : wordCorrections)
					{
						corection += W.second;
						corection += " ";
					}
					corection.erase(corection.end() - 1, corection.end());
					corection += "}";
					correctionOut << corection;
				}
				else if (wordCorrections.size() == 1)
				{
					for (auto& W : wordCorrections)
					{
						corection += W.second;
					}
					correctionOut << corection;
				}
			}
			bufferWordCorrections[word] = corection;
		}
	}
}

bool SpellChecker::checkCorectionsWhitOneEdits(const std::string& word)
{
	bool result = false;
	if (word.length() > 1)
		result |= checkLenLessByOne(word);
	result |= checkLenMoreByOne(word);
	return result;
}

bool SpellChecker::checkCorectionsWhitTwoEdits(const std::string& word)
{
	bool result = false;
	result |= checkEqualLen(word);
	result |= checkLenLessByTwo(word);
	result |= checkLenMoreByTwo(word);
	return result;
}

bool SpellChecker::checkEqualLen(const std::string& word)
{
	bool result = false;
	int wordLen = word.size();
	if (wordLen != 1)
	{
		int dicLen = wordLen;
		string dicWord;
		int countError;
		bool prevError;

		for (auto& wordPair : dictionary[dicLen])
		{
			countError = 0;
			prevError = false;
			dicWord = wordPair.first;
			for (uint8_t iW = 0, iDic = 0;
				iW < dicLen &&
				iDic < dicLen &&
				countError < 3;
				iDic++, iW++)
			{
				if (!InsensitiveCaseCompareChar(word[iW], dicWord[iDic]))
				{
					if (prevError)
					{
						if (!(InsensitiveCaseCompareChar(word[iW - 1], dicWord[iDic]) && countError < 2))
						{
							countError = 5;
							break;
						}
					}
					else
					{
						prevError = true;
						countError++;
					}
				}
				else
				{
					prevError = false;
				}
			}
			if (countError == 1)
			{
				result = true;
				wordCorrections[wordPair.second] = wordPair.first;
			}
		}
	}
	return result;
}

bool SpellChecker::checkLenLessByTwo(const std::string& word)
{


	bool result = false;
	int wordLen = word.size();
	if (wordLen > 2)
	{
		int dicLen = wordLen - 2;

		string dicWord;
		int countError;
		bool prevError;
		int needCheck;

		for (auto& wordPair : dictionary[dicLen])
		{
			countError = 0;
			prevError = false;
			dicWord = wordPair.first;
			needCheck = wordLen;
			for (uint8_t iW = 0, iDic = 0;
				iW < wordLen &&
				iDic < dicLen &&
				countError < 3;
				iW++)
			{
				if (!InsensitiveCaseCompareChar(word[iW], dicWord[iDic]))
				{
					if (prevError)
					{
						if (iW > 1)
						{
							if (InsensitiveCaseCompareChar(word[iW], word[iW - 1], word[iW - 2]))
							{
								prevError = false;
								countError++;
							}
							else
							{
								countError = 5;
								break;
							}
						}
						else
						{
							countError = 5;
							break;
						}
					}
					else
					{
						prevError = true;
						countError++;
					}
				}
				else
				{
					prevError = false;
					iDic++;
				}
				needCheck--;
			}
			if (needCheck == 2 &&
				countError == 0)
			{
				if (InsensitiveCaseCompareChar(word[wordLen - 2], dicWord[dicLen - 1]))
				{
					needCheck--;
						countError++;
				}
				else 
				{
					countError = 5;
				}
			}

			if (((countError + needCheck) < 3) && !(prevError && needCheck) && needCheck != 2)
			{
				result = true;
				wordCorrections[wordPair.second] = wordPair.first;
			}
		}
	}
	return result;
}

bool SpellChecker::checkLenMoreByTwo(const std::string& word)
{
	bool result = false;
	int wordLen = word.size();
	int dicLen = wordLen + 2;
	string dicWord;
	int countError;
	bool prevError;
	int needCheck;

	for (auto& wordPair : dictionary[dicLen])
	{
		countError = 0;
		prevError = false;
		dicWord = wordPair.first;
		needCheck = dicLen;
		for (uint8_t iW = 0, iDic = 0;
			iW < wordLen &&
			iDic < dicLen &&
			countError < 3;
			iDic++)
		{
			if (!InsensitiveCaseCompareChar(word[iW], dicWord[iDic]))
			{
				if (prevError)
				{
					if (iDic > 1) {
						if (InsensitiveCaseCompareChar(dicWord[iDic], dicWord[iDic - 1], dicWord[iDic - 2]))
						{
							prevError = false;
							countError++;
						}
						else
						{
							countError = 5;
							break;
						}
					}
					else
					{
						countError = 5;
						break;
					}
				}
				else
				{
					prevError = true;
					countError++;
				}
			}
			else
			{
				prevError = false;
				iW++;
			}
			needCheck--;
		}
		if (needCheck == 2 && countError == 0)
		{
			if (InsensitiveCaseCompareChar(word[wordLen - 1], dicWord[dicLen - 2]))
			{
				needCheck--;
				countError++;
			}
			else {
				countError = 5;
			}
		}

		if (((countError + needCheck) < 3) &&
			!(prevError && needCheck))
		{
			result = true;
			wordCorrections[wordPair.second] = wordPair.first;
		}
	}
	return result;
}
bool SpellChecker::checkLenLessByOne(const std::string& word)
{
	bool result = false;
	int wordLen = word.size();
	int dicLen = wordLen - 1;
	string dicWord;
	int countError;
	int needCheck;

	for (auto& wordPair : dictionary[dicLen])
	{
		countError = 0;
		dicWord = wordPair.first;
		needCheck = wordLen;
		for (uint8_t iW = 0, iDic = 0;
			iW < wordLen &&
			iDic < dicLen &&
			countError < 2;
			iW++)
		{
			if (!InsensitiveCaseCompareChar(word[iW], dicWord[iDic]))
			{
				countError++;
			}
			else
			{
				iDic++;
			}
			needCheck--;
		}
		if ((countError + needCheck) < 2)
		{
			result = true;
			wordCorrections[wordPair.second] = wordPair.first;
		}
	}
	return result;
}

bool SpellChecker::checkLenMoreByOne(const std::string& word)
{
	bool result = false;
	int wordLen = word.size();
	int dicLen = wordLen + 1;
	string dicWord;
	int countError;
	int needCheck;

	for (auto& wordPair : dictionary[dicLen])
	{
		countError = 0;
		dicWord = wordPair.first;
		needCheck = wordLen;
		bool prevError = false;
		for (uint8_t iW = 0, iDic = 0;
			iW < wordLen &&
			iDic < dicLen &&
			countError < 2;
			iDic++)
		{
			if (!InsensitiveCaseCompareChar(word[iW], dicWord[iDic]))
			{
				prevError = true;
				countError++;
			}
			else
			{
				prevError = false;
				iW++;
			}
			needCheck--;
		}
		if ((countError + needCheck) < 2 &&
			!(needCheck && prevError))
		{
			result = true;
			wordCorrections[wordPair.second] = wordPair.first;
		}
	}
	return result;
}
