#pragma once


#include "Vector2.h"
#include "Body.h"
#include "Pheromones.h"
#include "SensorDriver.h"




class Ant
{

public:
	Ant(int id, int x, int y, float rot, PheromoneMap* pheromoneMap);

	void process();
	void update();


	Body getBody();



	SensorDriver* sensorDriver;
private:


	PheromoneMap* pheromoneMap;

	
	
	BodyDriver* bodyDriver;


	//Body;





};

