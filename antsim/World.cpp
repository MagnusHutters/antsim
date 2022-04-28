#include "World.h"

#include "AntContainer.h"
#include "Pheromones.h"

#include <thread>

World::World(std::string name, int numAnts, int sizeX, int sizeY)
{

	pheromoneMap = new PheromoneMap(sizeX, sizeY);

	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->name = name;
	jobMap = new JobMap();
	jobFactory = new JobFactory(jobMap);

	jobFactory->createJobChain(2, true);

	antContainer = new AntContainer(numAnts, sizeX, sizeY, pheromoneMap, jobMap);

	

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
	return MAP_WIDTH;
}

float World::getPheromone(int x, int y, int id, bool positive)
{
	return pheromoneMap->getPheromone(x,y,id,positive);
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

Ant* World::getAnt(int id)
{
	return antContainer->ants[id];
}

void World::worldThread() {

	

	//while (1) {
		//update();
	//}
}

void World::update() {

	pheromoneMap->setReadOnly();

	antContainer->processAnts();

	doDebugLogger();

	pheromoneMap->setWriteOnly();

	antContainer->updateAnts();
	pheromoneMap->doDecayPheromones();

	jobMap->updateAll();

	pheromoneMap->setReadOnly();

}

void World::doDebugLogger()
{

	//antLog.push(std::to_string(antContainer->ants[0]->val3));

}
