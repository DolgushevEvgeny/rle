// rle.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>

#define MAX_LENGTH 255

using namespace std;

map<int, char> FillMap(void)
{
	map <int, char> numbers;
	for (int i = 0; i < 10; i++)
	{
		numbers[i] = 48 + i;
	}
	return numbers;
}

void WriteToFile(ofstream &file, const int &count, const char &symbol)
{
	file << count << symbol;
}

bool CheckOnDigit(const int &symbol, map<int, char> &digits)
{
	int compareCounter = 0;
	for (int i = 0; i < 10; ++i)
	{
		if (digits[i] == symbol)
		{
			compareCounter++;
		}
	}
	if (compareCounter == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CompileNumber(const int &count, const char &symbol, int &result)
{
	if (count == 0)
	{
		result += symbol - 48;
	}

	if (count > 0)
	{
		result = result * 10 + (symbol - 48);
	}
}

void Pack_File(const string &inputFileName, const string &outputFileName)
{
	ifstream inputFile;
	inputFile.open(inputFileName, ifstream::in);

	if (!(inputFile.good()))
	{
		exit(1);
	}

	ofstream outputFile;
	outputFile.open(outputFileName, ofstream::out);

	if (!(outputFile.good()))
	{
		exit(1);
	}
	
	char symbol, originalSymbol; int digitsCount = 0;

	while (!inputFile.eof())
	{
		inputFile.get(symbol);

		if (digitsCount == 0)
		{
			originalSymbol = symbol;
		}

		if (symbol == ' ')
		{
			continue;
		}

		if (symbol == originalSymbol)
		{
			digitsCount++;
		}
		else
		{
			if (digitsCount <= MAX_LENGTH)
			{
				WriteToFile(outputFile, digitsCount, originalSymbol);
				originalSymbol = symbol;
				digitsCount = 1;
			}
			else
			{
				while (digitsCount > MAX_LENGTH)
				{
					outputFile << MAX_LENGTH;
					digitsCount -= MAX_LENGTH;
				}
				WriteToFile(outputFile, digitsCount, originalSymbol);
				originalSymbol = symbol;
				digitsCount = 1;
			}
		}
	}

	if (digitsCount - 1)
	{
		WriteToFile(outputFile, digitsCount - 1, originalSymbol);
	}
}

void Unpack_File(const string &inputFileName, const string &outputFileName)
{
	ifstream inputFile;
	inputFile.open(inputFileName, ifstream::in);

	if (!(inputFile.good()))
	{
		exit(1);
	}

	ofstream outputFile;
	outputFile.open(outputFileName, ofstream::out);

	if (!(outputFile.good()))
	{
		exit(1);
	}

	map<int, char> digits = FillMap();
	char symbol;
	int digitsCounter = 0, result = 0, subResult = 0;
	bool isNotError;

	while (!inputFile.eof())
	{
		inputFile.get(symbol);
		isNotError = CheckOnDigit(symbol, digits);
		if (isNotError)
		{
			if (digitsCounter < 3)
			{
				CompileNumber(digitsCounter, symbol, result);
				digitsCounter++;
			}
			else
			{
				subResult += result;
				digitsCounter = 0;
				result = 0;
				CompileNumber(digitsCounter, symbol, result);
				digitsCounter++;
			}
		}
		else
		{
			for (int i = 0; i < subResult + result; i++)
			{
				outputFile << symbol;
			}
			subResult = 0;
			result = 0;
		}
	}	
}

int main(int argc, char* argv[])
{
	string mode = argv[1];
	string inputFileName = argv[2], outputFileName = argv[3];
	
	if (mode == "pack")
	{
		Pack_File(inputFileName, outputFileName);
		return 0;
	}
	if (mode == "unpack")
	{
		Unpack_File(inputFileName, outputFileName);
		return 0;
	}
	else
	{
		printf("Error: Unknown command to work with files!\n");
		return 1;
	}
}

