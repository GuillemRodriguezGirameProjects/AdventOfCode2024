// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <cmath>

void ReadFromFile(std::string aFileName, std::map<unsigned, unsigned>& aMap, std::multiset<unsigned>& aSet)
{
	std::ifstream InputFile(aFileName);
	std::string IdLine; 
	while (std::getline(InputFile, IdLine))
	{
		unsigned SeparatorPosition = IdLine.find("  ");
		unsigned MapNumber = std::atoi(IdLine.substr(0,SeparatorPosition).c_str());
		unsigned SetNumber = std::atoi(IdLine.substr(SeparatorPosition + 3, IdLine.size()-1).c_str());

		if (aMap.find(MapNumber) != aMap.end())
		{
			aMap[MapNumber] = aMap[MapNumber] + aMap.count(MapNumber);
		}
		else
		{
			aMap[MapNumber] = 1;
		}

		aSet.insert(std::atoi(IdLine.substr(SeparatorPosition + 3, IdLine.size()-1).c_str()));
	}

	InputFile.close();
}

unsigned CalculateSimilarityScore(std::map<unsigned, unsigned>& aMap, std::multiset<unsigned>& aSet)
{
	unsigned SimilarityScore = 0;
	for(auto Pair : aMap)
	{
		SimilarityScore += Pair.first * Pair.second * aSet.count(Pair.first);
	}
	return SimilarityScore;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter a file directory" << std::endl;
		return 0;
	}

	std::string InputFileName(argv[1]);
	std::map<unsigned, unsigned> Map;
	std::multiset<unsigned> Set;

	ReadFromFile(InputFileName, Map, Set);

	std::cout << "The total distance is: " <<  CalculateSimilarityScore(Map, Set) << std::endl;
	return 0;
}