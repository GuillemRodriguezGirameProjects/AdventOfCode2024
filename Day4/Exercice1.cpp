// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>

#include <regex>
#include <vector>
#include <list>

std::list<std::pair<int, int>> POSSIBLE_STEPS = std::list<std::pair<int, int>>
{
	//X	  Y
	{-1, -1},	//UP_LEFT_STEP
	{-1, 0},	//UP_STEP
	{-1, 1},	//UP_RIGHT_STEP
	{0,	1},		//RIGHT_STEP
	{1,	1},		//DOWN_RIGHT_STEP
	{1, 0},		//DOWN_STEP
	{1,	-1},	//DOWN_LEFT_STEP
	{0,	-1}		//LEFT_STEP
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

bool IsXMas(const std::vector<std::string>& aInput, const std::string& aWordToFind, int aCol, int aRow, const std::pair<int, int>& aStep)
{
	const int WORD_SIZE = aWordToFind.size();

	for(int i = 1; i < WORD_SIZE; ++i)
	{
		int RowStep = aRow + aStep.first*i;
		int ColumnStep = aCol + aStep.second*i;
		
		if(RowStep < 0 || ColumnStep < 0 || RowStep >= aInput.size()  || ColumnStep >= aInput[0].size()) 
		{
			// std::cout << "	     " << "BRO NO ME CUENTES TU VIDA" << std::endl;
			return false;
		}

		// std::cout << "	 " << RowStep << ", " << ColumnStep << ": ";
		// std::cout << "	 " << aWordToFind[i] << ", " << aInput[RowStep][ColumnStep] << std::endl;

		if (aWordToFind[i] != aInput[RowStep][ColumnStep])
		{
			return false;
		}
	}

	return true;
}

int FindOcurrencesInMatrix(const std::vector<std::string>& aInput, const std::string& aWordToFind, int aCol, int aRow)
{
	if (aWordToFind[0] != aInput[aRow][aCol]) return 0;

	// std::cout << "INICI: " << aRow << ", " << aCol << std::endl;

	int Ocurrences = 0;
	for(auto Step : POSSIBLE_STEPS)
	{	
		if (IsXMas(aInput, aWordToFind, aCol, aRow, Step))
		{
			++Ocurrences;
		}
	}
	
	return Ocurrences;
}

int ProcessInput(const std::vector<std::string>& aInput, const std::string aWordToFind)
{
	const int WORD_SIZE = aWordToFind.size();
	const int COLUMN_SIZE = aInput[0].size();
	const int ROW_SIZE = aInput.size();

	int XMASCounter = 0;
	
	for(int Row = 0; Row < ROW_SIZE; ++Row)
	{
		for(int Col = 0; Col <= COLUMN_SIZE; ++Col)
		{
			XMASCounter += FindOcurrencesInMatrix(aInput, aWordToFind, Col, Row);
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
	int TotalMultiplications = ProcessInput(InputContent, "XMAS");
	std::cout << "Multiplication Total? : " <<  TotalMultiplications << std::endl;
	return 0;
}