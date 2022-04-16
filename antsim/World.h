#pragma once

#include <string>
#include <vector>
#include <queue>



#include "Ant.h"
#include "AntContainer.h"
#include "Pheromones2.h"
#include "JobFactory.h"
#include "Job.h"
#include "EntityMap.h"
#include "Graphic.h"

class World
{


public:
	World(std::string name,int numAnts, int sizeX, int sizeY); //size must be in binary sequense 2^x

	void start(int frequency = 0);
	void stop();
	void setFrequency(int frequency);


	int getSizeX();
	int getSizeY();
	int getPheromoneSize();
	float getPheromone(int x, int y, int id, bool positive);
	const std::unordered_map<int, EntityMap<Job>::Entity*>& getJobPositions() {
		return jobMap->getEntities();
	}


	Vector2 getSensorLocation(bool side, int ant) {
		if (side) {
			return antContainer->ants[ant]->sensorDriver->getPrimaryLeft().vector;
		}
		else {
			return antContainer->ants[ant]->sensorDriver->getPrimaryRight().vector;
		}
	}


	std::vector<Body> getAntBodies();
	Ant* getAnt(int id);

	void worldThread();

	void update();

	void doDebugLogger();
	int getNumAnts()
	{
		return antContainer->ants.size();
	}

	std::queue<std::string> worldLog;
	std::queue<std::string> antLog;

private:
	std::string name;
	int frequency;


	int sizeX, sizeY;


	EntityMap<Job>* jobMap;
	JobFactory* jobFactory;
	AntContainer* antContainer;
	PheromoneMap* pheromoneMap;
	

	

	


};

