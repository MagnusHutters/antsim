#pragma once


#include <vector>
#include <list>


//Predefines
class Ant;

#include "Ant.h"
#include "EntityMap.h"
#include "Job.h"


class AntContainer
{



public:
	AntContainer(int numAnts, int sizeX, int sizeY, PheromoneMap* pheromoneMap, EntityMap<Job>* jobMap);




	




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
	
	EntityMap<Job>* jobMap;
	PheromoneMap* pheromoneMap;



};

