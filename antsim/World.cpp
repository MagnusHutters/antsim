#include "World.h"

#include "AntContainer.h"
#include "Pheromones.h"

#include <thread>

World::World(std::string name, int numAnts, int sizeX, int sizeY)
{

	pheromoneMap = new PheromoneMap(sizeX, sizeY);
	antContainer = new AntContainer(numAnts, sizeX, sizeY, pheromoneMap);
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->name = name;



	

}

int World::getSizeX()
{
	return sizeX;
}

int World::getSizeY()
{
	return sizeY;
}

int World::getPheromoneSize()
{
	return pheromoneMap->size;
}

float World::getPheromone(int x, int y, int id, bool positive)
{
	return pheromoneMap->innerPheromones[x][y]->getPheromone(id, positive);
}


std::vector<Body> World::getAntBodies()
{

	std::vector<Body> bodies;
	
	bodies.reserve(antContainer->ants.size());

	for (auto ant : antContainer->ants)
	{
		bodies.push_back(ant->getBody());
	}


	return bodies;
}

void World::worldThread() {

	

	//while (1) {
		//update();
	//}
}

void World::update() {

	pheromoneMap->doDecayPheromones();

	antContainer->processAnts();
	antContainer->updateAnts();

}
