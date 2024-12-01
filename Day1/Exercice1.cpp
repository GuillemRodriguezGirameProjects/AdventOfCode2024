// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <cmath>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter a file directory" << std::endl;
		return 0;
	}

	std::string InputFileName(argv[1]);
	std::ifstream InputFile(InputFileName);

	std::multiset<unsigned> FirstList;
	std::multiset<unsigned> SecondList;
	
	std::string IdLine; 
	while (std::getline(InputFile, IdLine))
	{
		unsigned SeparatorPosition = IdLine.find("  ");
		FirstList.insert(std::atoi(IdLine.substr(0,SeparatorPosition).c_str()));
		SecondList.insert(std::atoi(IdLine.substr(SeparatorPosition + 3, IdLine.size()-1).c_str()));
	}

	InputFile.close();

	unsigned TotalDistance = 0;
	std::multiset<unsigned>::iterator FirstListIterator = FirstList.begin();
	std::multiset<unsigned>::iterator SecondListIterator = SecondList.begin();
	while (FirstListIterator != FirstList.end() && SecondListIterator != SecondList.end()) 
	{
		int Distance = *FirstListIterator - *SecondListIterator;
		TotalDistance += std::abs(Distance);
		FirstListIterator++;
		SecondListIterator++;
	}

	std::cout << "The total distance is: " <<  TotalDistance << std::endl;
	return 0;
}