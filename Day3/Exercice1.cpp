// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>

#include <regex>

std::regex GetCorrectMultiplier("mul\\([0-9]{1,3},[0-9]{1,3}\\)");
std::regex GetNumbers("[0-9]{1,3}");

std::string ReadInput(std::string aInputFileName)
{
	std::string ReturnedInput;
	std::string LineRead;
	std::ifstream InputFile(aInputFileName);
	while (std::getline(InputFile, LineRead))
	{
		ReturnedInput += LineRead;
	}

	InputFile.close();

	return ReturnedInput;
}

int ProcessMultiplication(const std::string& aReadInput)
{
	int ReturnValue = 1;

	std::smatch NumbersToMultiply;
	std::string AuxString = aReadInput;

	while (std::regex_search (AuxString, NumbersToMultiply, GetNumbers))
	{
		std::cout << NumbersToMultiply.str() << " ";
		ReturnValue *= std::atoi(NumbersToMultiply.str().c_str());
		AuxString = NumbersToMultiply.suffix().str();
	}
	
	std::cout << std::endl;

	return ReturnValue;
}

int ProcessInput(const std::string& aReadInput)
{
	int TotalMultiplication = 0;

	std::smatch MatchingMultiplications;
	std::string AuxString = aReadInput;
	while (std::regex_search (AuxString, MatchingMultiplications, GetCorrectMultiplier))
	{
		for (auto Result : MatchingMultiplications) 
		{
			std::cout << Result.str() << " ";
			TotalMultiplication += ProcessMultiplication(Result.str());
		}
		std::cout << std::endl;

		AuxString = MatchingMultiplications.suffix().str();
	}

	return TotalMultiplication;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter a file directory" << std::endl;
		return 0;
	}

	std::string InputFileName(argv[1]);
	const std::string InputContent = ReadInput(InputFileName);
	int TotalMultiplications = ProcessInput(InputContent);
	std::cout << "Multiplication Total? : " <<  TotalMultiplications << std::endl;
	return 0;
}