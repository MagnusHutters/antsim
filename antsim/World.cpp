#include "World.h"

#include "AntContainer.h"
#include "Pheromones.h"



World::World(std::string name, int numAnts, int sizeX, int sizeY)
{

	pheromoneMap = new PheromoneMap(sizeX, sizeY);
	antContatiner = new AntContainer(numAnts, sizeX, sizeY, pheromoneMap);
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

void World::update() {

	pheromoneMap->doDecayPheromones();

	antContatiner->processAnts();
	antContatiner->updateAnts();

}
