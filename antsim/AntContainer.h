#pragma once


#include <vector>
#include <list>



//Predefines
class Ant;

#include "Ant.h"


class AntContainer
{



public:
	AntContainer(int numAnts, int sizeX, int sizeY, MapContainer m);




	




	void processAnts();
	void updateAnts();

	void setNumAnts(int numAnts);
	void createAnts(int numAnts);
	void deleteAnts(int numAnts);
	std::list<Ant*> getAvailableAnts()
	{
		std::list<Ant*> returnList;
		for (Ant* ant : ants)
		{
			int state = ant->getState();

			if(state==states::FindJob || state==states::None)
			{
				returnList.push_back(ant);
			}
		}
		return returnList;
	}


	std::vector<Ant*> ants;
private:

	void createAnt();
	void deleteAnt();

	int sizeX, sizeY;
	int antIdCount;
	int antCount;


	MapContainer m;



};

