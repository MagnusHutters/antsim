#include "Ant.h"
#include <stdlib.h>


Ant::Ant(int id, int x, int y, float rot, PheromoneMap* pheromoneMap)
{
	this->pheromoneMap = pheromoneMap;
	


	bodyDriver = new BodyDriver(x,y,rot);

	sensorDriver = new SensorDriver(bodyDriver, this->pheromoneMap);
	sensorDriver->setSensePheromone(EXPLORED);

	//body = Body(x, y, rot);
	
}

void Ant::process()
{
	
	bodyDriver->process();
	sensorDriver->doSense();


}

void Ant::update()
{

	pheromoneMap->addPheromone((int)bodyDriver->body.pos.x, (int)bodyDriver->body.pos.y, EXPLORED, true, 1.0);


	float val1 = sensorDriver->getPheromoneSenseFrontLeft(true);
	float val2 = sensorDriver->getPheromoneSenseFrontRight(true);
	float val3 = val1 - val2;
	bodyDriver->addDesiredMotion(Vector2(1, val3));

	/*
	if (sensorDriver->getPheromoneSenseFrontLeft(true) > sensorDriver->getPheromoneSenseFrontRight(true)) {
		bodyDriver->addDesiredMotion(Vector2(1, 1));

	}
	else {
		bodyDriver->addDesiredMotion(Vector2(1, -1));
	}*/

	bodyDriver->update();


}

Body Ant::getBody()
{
	return bodyDriver->body;
}
