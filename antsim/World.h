#pragma once

#include <string>
#include <vector>
#include <queue>



#include "Ant.h"
#include "AntContainer.h"
#include "Pheromones2.h"

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

	Vector2 getSensorLocation(bool side, int ant) {
		if (side) {
			return antContainer->ants[ant]->sensorDriver->getPrimaryLeft().vector;
		}
		else {
			return antContainer->ants[ant]->sensorDriver->getPrimaryRight().vector;
		}
	}


	std::vector<Body> getAntBodies();

	void worldThread();

	void update();

	void doDebugLogger();

	std::queue<std::string> worldLog;
	std::queue<std::string> antLog;

private:
	std::string name;
	int frequency;


	int sizeX, sizeY;



	AntContainer* antContainer;
	PheromoneMap* pheromoneMap;
	

	

	


};

