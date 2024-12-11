// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <string>

#include <regex>
#include <vector>
#include <set>

enum eDirection
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

std::map<eDirection, std::pair<int, int>> DIRECTIONS = std::map<eDirection, std::pair<int, int>>
{
	//X	  Y
	{UP, {-1, 0}},	//UP_LEFT_STEP
	{RIGHT, {0, 1}},	//UP_LEFT_STEP
	{DOWN, {1, 0}},	//UP_LEFT_STEP
	{LEFT, {0, -1}},	//UP_LEFT_STEP
};

typedef std::pair<int, int> Object; //Coordenades es troba l'objecte amb el que s'ha colisionat.

typedef std::pair<Object, eDirection> Collision;
	//Objecte amb el que s'ha colisionat.
	//Direcció en la que s'ha colisionat.

struct sGuard
{
	Object mPosition; //Posició en la que es troba el guarda.
	eDirection mDirection;	//Direcció en que es mou
};

std::pair<int, int> MAP_SIZE = std::pair<int, int>(0,0); //Tamany del mapa
bool BUCLE_INFINIT = false;
std::set<std::pair<int, int>> VISITED_COORDINADES;

sGuard ReadInput(std::string aInputFileName, std::set<Object>& ObjectSet)
{
	std::ifstream InputFile(aInputFileName);
	std::string LineRead;

	sGuard ReturnGuard;
	int CurrentRow = 0;
	while (std::getline(InputFile, LineRead))
	{
		MAP_SIZE.second = MAP_SIZE.second < LineRead.size() ? LineRead.size() : MAP_SIZE.second;

		for (int i = 0; i < LineRead.size(); i++)
		{
			char CurrentCharacter = LineRead[i];
			switch (CurrentCharacter)
			{
				case '#':
				{
						Object CurrentObject(std::pair<int, int>(CurrentRow, i));
						ObjectSet.emplace(CurrentObject);
						// std::cout << "Object (" << CurrentRow << "," << i << ")" << std::endl;
				}
				break;
				case '^':
				{
						ReturnGuard.mPosition = Object(std::pair<int, int>(CurrentRow, i));
						ReturnGuard.mDirection = UP;
				}
					break;
				case '>':
				{
						ReturnGuard.mPosition = Object(std::pair<int, int>(CurrentRow, i));
						ReturnGuard.mDirection = RIGHT;
				}
				break;
				case 'v':
				{
						ReturnGuard.mPosition = Object(std::pair<int, int>(CurrentRow, i));
						ReturnGuard.mDirection = DOWN;
				}
				break;
				case '<':
				{
						ReturnGuard.mPosition = Object(std::pair<int, int>(CurrentRow, i));
						ReturnGuard.mDirection = LEFT;
				}
				break;
			}
		}
		
		CurrentRow++;
	}

	MAP_SIZE.first = CurrentRow;

	// std::cout << "Guard (" << ReturnGuard.mPosition.first << "," << ReturnGuard.mPosition.second << ")" << std::endl;
	// std::cout << "MAP_SIZE (" << MAP_SIZE.first << "," << MAP_SIZE.second << ")" << std::endl;


	InputFile.close();
	return ReturnGuard;	
}

bool IsInsideMap(const Object aObject)
{
	return aObject.first >= 0 && aObject.first < MAP_SIZE.first
			&&
			aObject.second >= 0 && aObject.second < MAP_SIZE.second;
}

bool CheckCollision(std::set<Object>& aObjectSet, sGuard& aGuard, std::set<Collision>& aCollisions)
{
	std::pair<int, int> NextPosition;
	NextPosition.first = DIRECTIONS.at(aGuard.mDirection).first + aGuard.mPosition.first;
	NextPosition.second = DIRECTIONS.at(aGuard.mDirection).second + aGuard.mPosition.second;

	for (auto Object : aObjectSet)
	{
		// std::cout << "OBJECT (" << Object.first << "," << Object.second << ")" << std::endl;

		if(NextPosition == Object)
		{
			Collision AuxCollision;
			AuxCollision.first = Object;
			AuxCollision.second = aGuard.mDirection;

			if (aCollisions.count(AuxCollision))
			{
				std::cout << "BUCLE INFINIT WACHOOOOOO" << std::endl;
				BUCLE_INFINIT = true;
			}
			else
			{
				aCollisions.emplace(AuxCollision);
			}

			return true;
		}
	}

	return false;
}

void MoveGuard(std::set<Object>& aObjectSet, sGuard& aGuard, std::set<Collision>& aCollisions)
{
	int StepsTaken = 0;
	while (IsInsideMap(aGuard.mPosition) && !CheckCollision(aObjectSet, aGuard, aCollisions) && !BUCLE_INFINIT)
	{
		if(!VISITED_COORDINADES.count(aGuard.mPosition)) VISITED_COORDINADES.emplace(aGuard.mPosition);

		aGuard.mPosition.first += DIRECTIONS.at(aGuard.mDirection).first;
		aGuard.mPosition.second += DIRECTIONS.at(aGuard.mDirection).second;

		std::cout << "GUARDA (" << aGuard.mPosition.first << "," << aGuard.mPosition.second << ")" << std::endl;

	}
	
	std::cout << "------" << std::endl;	
}

void RotateGuard(sGuard& aGuard)
{
	switch (aGuard.mDirection)
	{
		case UP:
			aGuard.mDirection = RIGHT;
			break;
		case RIGHT:
			aGuard.mDirection = DOWN;
			break;
		case DOWN:
			aGuard.mDirection = LEFT;
			break;
		case LEFT:
			aGuard.mDirection = UP;
			break;
	}
}

void CountGuardSteps(std::set<Object>& aObjectSet, sGuard aGuard)
{
	std::set<Collision> Collisions;

	while(IsInsideMap(aGuard.mPosition) && !BUCLE_INFINIT)
	{
		MoveGuard(aObjectSet, aGuard, Collisions);
		RotateGuard(aGuard);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Please enter a file directory" << std::endl;
		return 0;
	}

	std::string InputFileName(argv[1]);

	std::set<Object> ObjectSet;

	sGuard Guard = ReadInput(InputFileName, ObjectSet);
	CountGuardSteps(ObjectSet, Guard);


	std::cout << "TotalSteps: " << VISITED_COORDINADES.size() << std::endl;
	return 0;
}