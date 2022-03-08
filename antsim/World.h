#pragma once

#include <string>
#include <vector>



#include "Ant.h"
#include "AntContainer.h"
#include "Pheromones.h"

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


	std::vector<Body> getAntBodies();

	void worldThread();

	void update();

	

private:
	std::string name;
	int frequency;


	int sizeX, sizeY;



	AntContainer* antContainer;
	PheromoneMap* pheromoneMap;
	

	

	


};

