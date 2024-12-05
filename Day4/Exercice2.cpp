// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>

#include <regex>
#include <vector>
#include <list>

std::list<std::pair<int, int>> POSSIBLE_STEPS = std::list<std::pair<int, int>>
{
	{-1,-1},	//UP_LEFT_STEP
	{-1,1},		//UP_RIGHT_STEP
	{1,1},		//DOWN_RIGHT_STEP
	{1,-1},		//DOWN_LEFT_STEP
};

std::vector<std::string> ReadInput(std::string aInputFileName)
{
	std::vector<std::string> ReturnedInput;
	std::string LineRead;
	std::ifstream InputFile(aInputFileName);
	
	while (std::getline(InputFile, LineRead))
	{
		ReturnedInput.push_back(LineRead);
	}

	InputFile.close();
	return ReturnedInput;
}

bool IsXMas(const std::vector<std::string>& aInput, const std::string& aWordToFind, const std::string& aReversedWordToFind, int aCol, int aRow)
{
	if(aWordToFind[1] != aInput[aRow][aCol]) return false;
	if(aRow-1 < 0 || aCol-1 < 0 || aRow+1 == aInput.size() || aCol+1 == aInput[0].size()) return false;

	std::string Diagonal1 = ""; 
	Diagonal1 = aInput[aRow-1][aCol-1];
	Diagonal1 += aInput[aRow][aCol];
	Diagonal1 += aInput[aRow+1][aCol+1];

	std::string Diagonal2 = "";
	Diagonal2 = aInput[aRow-1][aCol+1];
	Diagonal2 += aInput[aRow][aCol];
	Diagonal2 += aInput[aRow+1][aCol-1];


	bool IsMatch = 	(Diagonal1 == aWordToFind || Diagonal1 == aReversedWordToFind) &&
					(Diagonal2 == aWordToFind || Diagonal2 == aReversedWordToFind);

	if(IsMatch)
	{
		std::cout << "Diagonal1: " << Diagonal1 << std::endl;
		std::cout << "Diagonal2: " << Diagonal2 << std::endl;
	}
	
	return IsMatch;
}

int ProcessInput(const std::vector<std::string>& aInput, const std::string aWordToFind, const std::string aReversedWordToFind)
{
	const int WORD_SIZE = aWordToFind.size();
	const int COLUMN_SIZE = aInput[0].size();
	const int ROW_SIZE = aInput.size();

	int XMASCounter = 0;
	
	for(int Row = 0; Row < ROW_SIZE; ++Row)
	{
		for(int Col = 0; Col <= COLUMN_SIZE; ++Col)
		{
			XMASCounter += IsXMas(aInput, aWordToFind, aReversedWordToFind, Col, Row);
		}
	}

	return XMASCounter;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter a file directory" << std::endl;
		return 0;
	}

	std::string InputFileName(argv[1]);
	const std::vector<std::string> InputContent = ReadInput(InputFileName);
	int TotalMultiplications = ProcessInput(InputContent, "MAS", "SAM");
	std::cout << "Multiplication Total? : " <<  TotalMultiplications << std::endl;
	return 0;
}