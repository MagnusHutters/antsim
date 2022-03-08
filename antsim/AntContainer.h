#pragma once


#include <vector>
#include <list>


//Predefines
class Ant;

#include "Ant.h"


class AntContainer
{



public:
	AntContainer(int numAnts, int sizeX, int sizeY, PheromoneMap* pheromoneMap);




	




	void processAnts();
	void updateAnts();

	void setNumAnts(int numAnts);
	void createAnts(int numAnts);
	void deleteAnts(int numAnts);


	std::vector<Ant*> ants;
private:

	void createAnt();
	void deleteAnt();

	int sizeX, sizeY;
	int antIdCount;
	int antCount;
	
	PheromoneMap* pheromoneMap;



};

