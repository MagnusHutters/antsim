#pragma once

#include <string>
#include <vector>
#include <queue>



#include "Ant.h"
#include "AntContainer.h"
#include "Pheromones2.h"
#include "JobFactory.h"
#include "Job.h"
#include "Entity.h"
//#include "EntityMap.h"
#include "JobMap.h"
#include "Graphic.h"
#include "MapContainer.h"
#include "RigidBodyMap.h"

#include "ObstacleMap.h"
#include "TerrainFactory.h"
#include "CentralController.h"


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
	const std::unordered_map<int, Job*>& getJobPositions() {
		return jobMap->getEntities();
	}
	
	const std::array<std::array<bool, OBSTACLE_MAP_HEIGHT>, OBSTACLE_MAP_WIDTH>& getTerrain()
	{
		return mapContainer.obstacleMap->getTerrain();
	}

	Vector2 getSensorLocation(bool side, int ant) {
		if (side) {
			return antContainer->ants[ant]->sensorDriver->getPrimaryLeft().vector;
		}
		else {
			return antContainer->ants[ant]->sensorDriver->getPrimaryRight().vector;
		}
	}

	struct _getPaths
	{
		std::unordered_map<int, std::list<Path>>* paths;
	};
	_getPaths getPaths()
	{
		 return {pathArchive->getAllPaths()};
	}

	std::vector<Body> getAntBodies();
	Ant* getAnt(int id);

	void worldThread();

	void update(int tick);

	void doDebugLogger();
	int getNumAnts()
	{
		return antContainer->ants.size();
	}

	inline PheromoneActiveReturn getPheromoneActive(bool complete)
	{
		return pheromoneMap->getActivePheromones(complete);
	}
	inline void resetPheromoneToUpdate()
	{
		pheromoneMap->resetToUpdate();
	}

	std::array<float, MAP_CELLS> getObstructed()
	{
		return pheromoneMap->getObstrcuted();
	}

	std::queue<std::string> worldLog;
	std::queue<std::string> antLog;

private:
	friend class Logger;
	std::string name;
	int frequency;
	int counter = 0;

	int sizeX, sizeY;


	JobMap* jobMap;
	JobFactory* jobFactory;
	TerrainFactory* terrainFactory;


	AntContainer* antContainer;
	PheromoneMap* pheromoneMap;
	RigidBodyMap* bodyCollosionMap;
	ObstacleMap* obstacleMap;
	PathArchive* pathArchive;

	MapContainer mapContainer;


	CentralController* centralController;

	


};

