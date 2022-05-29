#include "World.h"

#include "AntContainer.h"
#include "Pheromones.h"

#include <thread>

#include "PathArchive.h"
#include "Timer.h"

World::World(std::string name, int numAnts, int sizeX, int sizeY)
{

	pheromoneMap = new PheromoneMap(sizeX, sizeY);

	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->name = name;
	jobMap = new JobMap();
	

	bodyCollosionMap=new RigidBodyMap();

	obstacleMap = new ObstacleMap();

	

	pathArchive = new PathArchive();
	mapContainer = { pheromoneMap,jobMap,bodyCollosionMap,obstacleMap,pathArchive };

	


	terrainFactory = new TerrainFactory(mapContainer);


	jobFactory = new JobFactory(mapContainer);

	//jobFactory->createJobChain(2, true);
	terrainFactory->tryCreateTerrain(2, false,true);
	terrainFactory->tryCreateTerrain(900, true,true);
	terrainFactory->tryCreateTerrain(100, true, false);
	//terrainFactory->progressTerrain(10000);
	jobFactory->createDynamicSuplyChainPyramid(1);

	jobFactory->createJobChain();

	antContainer = new AntContainer(numAnts, sizeX, sizeY, mapContainer);

	centralController = new CentralController(antContainer,jobMap);




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

void World::update(int tick) {

	Timer<> clock1;
	Timer<> clock2;
	Timer<> clock3;
	Timer<> clock4;
	Timer<> clock5;

	clock1.tick();
	
	

	pheromoneMap->setReadOnly();
	if(ANT_CONTROLLER==1)
	{
		centralController->update();
	}
	

	antContainer->processAnts();

	clock1.tock();
	clock2.tick();

	

	doDebugLogger();

	pheromoneMap->setWriteOnly();

	antContainer->updateAnts();

	clock2.tock();
	clock3.tick();
	if(counter++<10)
	{
		//terrainFactory->update();
		counter = 0;
	}
	

	pheromoneMap->doDecayPheromones();

	clock3.tock();
	clock4.tick();

	jobMap->updateAll();

	clock4.tock();
	clock5.tick();

	jobFactory->update();

	pheromoneMap->setReadOnly();
	clock5.tock();

	/*antLog.push(""
		+ std::string("Process ants dt: ")
		+ std::to_string((clock1.duration<>()).count())
		+ std::string("\t updateAnts dt: ")
		+ std::to_string((clock2.duration<>()).count())
		+ std::string("\t doDecayPheromones dt: ")
		+ std::to_string((clock3.duration<>()).count())
		+ std::string("\t updateAll dt: ")
		+ std::to_string((clock4.duration<>()).count())
		+ std::string("\t setReadOnly dt: ")
		+ std::to_string((clock5.duration<>()).count())

	);*/

}

void World::doDebugLogger()
{

	//antLog.push(std::to_string(antContainer->ants[0]->val3));

}
