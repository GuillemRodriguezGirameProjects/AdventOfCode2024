// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>

#include <regex>
#include <vector>
#include <set>

void PrintVector(std::vector<int>& aVec)
{
	for (auto Value : aVec)
	{
		std::cout << Value << ",";
	}
	std::cout << std::endl;
}


void ReadRules(std::ifstream& InputFile, std::multiset<std::pair<int, int>>& aRuleSet)
{	
	//Llegim fins acabar totes les regles
	std::string LineRead;
	std::getline(InputFile, LineRead);
	while (LineRead != "")
	{
		int SeparatorPosition = LineRead.find("|");
		std::string LeftNumber = LineRead.substr(0,SeparatorPosition);
		std::string RightNumber = LineRead.substr(SeparatorPosition+1,LineRead.size()-1);
		int Left = std::atoi(LeftNumber.c_str());
		int Right = std::atoi(RightNumber.c_str());
		aRuleSet.insert(std::pair<int, int>(Left, Right));
		std::getline(InputFile, LineRead);
	}
}

void ReadPages(std::ifstream& InputFile, std::vector<std::vector<int>>& aPageVector)
{	
	std::string LineRead;
	while (std::getline(InputFile, LineRead))
	{
		std::string RemainingString = LineRead;
		std::vector<int> AuxVector;
		while(RemainingString.size())
		{
			int SeparatorPosition = RemainingString.find(",");
			if(SeparatorPosition+1 >= RemainingString.size()-1) break;

			std::string LeftNumber = RemainingString.substr(0,SeparatorPosition);
			int Left = std::atoi(LeftNumber.c_str());
			AuxVector.push_back(Left);
			RemainingString = RemainingString.substr(SeparatorPosition+1,RemainingString.size()-1);
		}
		aPageVector.push_back(AuxVector);
	}
}

void ReadInput(std::string aInputFileName, std::multiset<std::pair<int, int>>& aRuleSet, std::vector<std::vector<int>>& aPageVector)
{
	std::ifstream InputFile(aInputFileName);
	
	ReadRules(InputFile, aRuleSet);
	ReadPages(InputFile, aPageVector);

	InputFile.close();
}

int IsCorrectPage(const std::multiset<std::pair<int, int>>& aRuleSet, const std::vector<int>& aManual, const int aIndex, const int Page)
{
	for (int i = aIndex+1; i < aManual.size(); i++)
	{
		// std::cout << Page << "|" << aManual[i] << std::endl;
		// Comprovar que x va davant de y (63|13)
		bool HasRule = aRuleSet.count(std::pair<int, int>(Page, aManual[i]));
		// std::cout << "HasRule: " << HasRule << std::endl;
		if (!HasRule)
		{
			// Comprovar que y no va davant de x (29|13)
			bool ViolatesRule = aRuleSet.count(std::pair<int, int>(aManual[i], Page));
			// std::cout << "ViolatesRule: " << ViolatesRule << std::endl;
			if (ViolatesRule)
			{
				return i;
			}
		}
	}
	
	return -1;
}

int IsCorrectManual(const std::multiset<std::pair<int, int>>& aRuleSet, const std::vector<int>& aManual)
{
	int ReturnValue = -1;
	int i = 0;
	bool Correct = true;
	while (i < aManual.size() && Correct)
	{
		Correct = IsCorrectPage(aRuleSet, aManual, i, aManual[i]) == -1;
		if(Correct) 
			++i;

		// std::cout << "Correct: " << Correct << std::endl;
	}

	// std::cout << "ReturnValue: " << ReturnValue << std::endl;
	return Correct ? -1 : i;
}

void RectifyPageOrder(const std::multiset<std::pair<int, int>>& aRuleSet, std::vector<int>& aManual)
{

	bool Correct = false;
	while (!Correct)
	{
		int Index = IsCorrectManual(aRuleSet, aManual);
		if (Index != -1)
		{			
			// PrintVector(aManual);
			std::vector<int>::iterator It = aManual.begin() + Index;
			std::rotate(It, It+1, aManual.end());
			// PrintVector(aManual);
		}
		else
		{
			Correct = true;
		}
	}
}


int RectifyManual(const std::multiset<std::pair<int, int>>& aRuleSet, const std::vector<int>& aManual)
{
	std::vector<int> AuxManual = aManual;
	int ReturnValue = -1;

	if(IsCorrectManual(aRuleSet, AuxManual) != -1)
	{
		RectifyPageOrder(aRuleSet, AuxManual);
		ReturnValue = AuxManual[(AuxManual.size() - 1)/2];
	}

	return ReturnValue;
}

int ProcessInput(const std::multiset<std::pair<int, int>>& aRuleSet, const std::vector<std::vector<int>>& aManualVector)
{
	int ReturnValue = 0;

	for (auto& Manual : aManualVector)
	{
		int Value = RectifyManual(aRuleSet, Manual);
		if(Value != -1)
		{
			ReturnValue += Value;
		}
	}

	return ReturnValue;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter a file directory" << std::endl;
		return 0;
	}

	std::string InputFileName(argv[1]);

	std::multiset<std::pair<int, int>> RuleSet;
	std::vector<std::vector<int>> PageVector;

	ReadInput(InputFileName, RuleSet, PageVector);
	int Result = ProcessInput(RuleSet, PageVector);

	std::cout << "Result: " << Result;
	return 0;
}