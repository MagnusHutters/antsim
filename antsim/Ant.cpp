#include "Ant.h"
#include <stdlib.h>




Ant::Ant(int id, int x, int y, float rot, PheromoneMap* pheromoneMap)
{
	this->pheromoneMap = pheromoneMap;
	this->id = id;


	bodyDriver = new BodyDriver(x,y,rot);

	sensorDriver = new SensorDriver(bodyDriver, this->pheromoneMap);

	actionDriver = new ActionDriver(sensorDriver, bodyDriver);

	sensorDriver->setSensePheromone(EXPLORED);

	
	actionDriver->setAction(new ActionFollowTrail(EXPLORED));

	//body = Body(x, y, rot);
	
}

void Ant::process()
{
	if (id == 0) {
		volatile int a = 1 + 1;
	}
	

	sensorDriver->doSense();

	actionDriver->calcAction();

	bodyDriver->process();




}

void Ant::update()
{

	pheromoneMap->addPheromone((int)bodyDriver->body.pos.x, (int)bodyDriver->body.pos.y, EXPLORED, true, 1.0);



	/*
	if (sensorDriver->getPheromoneSenseFrontLeft(true) > sensorDriver->getPheromoneSenseFrontRight(true)) {
		bodyDriver->addDesiredMotion(Vector2(1, 1));

	}
	else {
		bodyDriver->addDesiredMotion(Vector2(1, -1));
	}*/

	actionDriver->doAction();

	bodyDriver->update();


}

Body Ant::getBody()
{
	return bodyDriver->body;
}
