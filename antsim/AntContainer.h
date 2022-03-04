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




	std::vector<Body> getAntBodies();




	void processAnts();
	void updateAnts();

	void setNumAnts(int numAnts);
	void createAnts(int numAnts);
	void deleteAnts(int numAnts);



private:

	void createAnt();
	void deleteAnt();

	int sizeX, sizeY;
	int antIdCount;
	int antCount;
	std::vector<Ant*> ants;
	PheromoneMap* pheromoneMap;



};

