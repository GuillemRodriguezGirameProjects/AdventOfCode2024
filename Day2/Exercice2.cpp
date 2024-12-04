// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

void PrintReportWithErrorIndex(std::vector<unsigned>& aReport, int aIndex)
{
	std::cout << "Report [" << aIndex << "]:";
	for (auto ID : aReport)
	{
		std::cout << " " << ID;
	}
	std::cout << std::endl;
}

std::vector<std::vector<unsigned>> ReadInput(std::string aInputFileName)
{
	const std::string SEPARATOR = " ";
	std::vector<std::vector<unsigned>> InputContent;

	std::ifstream InputFile(aInputFileName);
	std::string IdLine; 
	while (std::getline(InputFile, IdLine))
	{
		std::vector<unsigned> LineInputVector;

		int SeparatorPosition = IdLine.find(SEPARATOR);
		while(IdLine.size())
		{
			unsigned ID = std::atoi(IdLine.substr(0,SeparatorPosition).c_str());
			LineInputVector.push_back(ID);

			if ( SeparatorPosition == -1)
			{
				break;
			}

			IdLine = IdLine.substr(SeparatorPosition+1, IdLine.size()-1);
			SeparatorPosition = IdLine.find(SEPARATOR);
		}
		
		if(!LineInputVector.empty())
		{
			InputContent.push_back(LineInputVector);
		}
	}

	InputFile.close();

	return InputContent;
}

bool IsSafe(std::vector<unsigned>& aReport)
{
	// Si aReport[0] - aReport[1] es negatiu, significa que el patró a seguir és Incremental, si és positiu llavors es Decremental
	bool Increasing = aReport[0] < aReport[1];
	for(unsigned Index = 0; Index < aReport.size() -1; ++Index)
	{
		int AuxDistance = std::abs(int(aReport[Index] - aReport[Index+1]));
		bool AuxIncreasing = aReport[Index] < aReport[Index+1];

		if(AuxDistance < 1 || AuxDistance > 3) return false;
		if(Increasing != AuxIncreasing) return false;
	}

	return true;
}

bool IsSafeWithDampener(std::vector<unsigned>& aReport, unsigned aMaxDepth, unsigned& aCurrentDepth)
{
	bool ReturnValue = IsSafe(aReport);
	if(ReturnValue)	return true;
	if(aCurrentDepth >= aMaxDepth) return false;

	for (unsigned Index = 0; Index < aReport.size(); ++Index)
	{
		std::vector<unsigned> NewReport;
		for (unsigned IndexToInsert = 0; IndexToInsert < aReport.size(); ++IndexToInsert)
		{
			if(Index != IndexToInsert) NewReport.push_back(aReport[IndexToInsert]);
		}

		if(IsSafeWithDampener(NewReport, aMaxDepth, ++aCurrentDepth)) return true;
	}

	return false;
}

unsigned CalculateNumberOfSafeReports(std::vector<std::vector<unsigned>>& aReportsVector)
{
	unsigned SafeReportCounter = 0;
	for (std::vector<unsigned>& Report : aReportsVector)
	{
		if(Report.size() < 2) continue;
		
		unsigned CurrentDepth = 0;
		if(IsSafeWithDampener(Report, 1, CurrentDepth))
		{
			++SafeReportCounter;
		}
	}

	return SafeReportCounter;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter a file directory" << std::endl;
		return 0;
	}

	std::string InputFileName(argv[1]);
	std::vector<std::vector<unsigned>> InputContent = ReadInput(InputFileName);
	unsigned SafeReports = CalculateNumberOfSafeReports(InputContent);
	std::cout << "How many reports are safe? : " <<  SafeReports << std::endl;
	return 0;
}