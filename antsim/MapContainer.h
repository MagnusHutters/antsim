#pragma once



class JobMap;
class ObstacleMap;
class RigidBodyMap;
class PheromoneMap;
class PathArchive;

class MapContainer
{
public:
	PheromoneMap* pheromoneMap;
	JobMap* jobMap;
	RigidBodyMap* bodyCollosionMap;
	ObstacleMap* obstacleMap;
	PathArchive* pathArchive;
};

