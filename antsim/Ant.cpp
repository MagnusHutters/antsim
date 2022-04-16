#include "Ant.h"
#include <stdlib.h>




Ant::Ant(int id, int x, int y, float rot, PheromoneMap* pheromoneMap, EntityMap<Job>* jobMap)
{
	val1 = 0;
	val2 = 0;
	val3 = 0;
	this->pheromoneMap = pheromoneMap;
	this->id = id;
	this->jobMap = jobMap;


	bodyDriver = new BodyDriver(x,y,rot, id);

	pheromoneDriver = new PheromoneDriver(pheromoneMap, bodyDriver);

	sensorDriver = new SensorDriver(bodyDriver, this->pheromoneMap, this->jobMap);

	actionDriver = new ActionDriver(sensorDriver, bodyDriver);

	taskDriver = new TaskChanger(sensorDriver, actionDriver, bodyDriver, pheromoneDriver);

	//taskDriver->setTask(new TaskFindJob());

	//sensorDriver->setPrimarySensorPheromone(EXPLORED);

	
	//actionDriver->setAction(new ActionFollowTrail(EXPLORED));
	//actionDriver->setAction(new ActionExplore(EXPLORED));

	//body = Body(x, y, rot);
	
}

void Ant::process()
{
	if (id == 0) {
		volatile int a = 1 + 1;
	}
	

	sensorDriver->doSense();

	actionDriver->calcAction();
	taskDriver->calcTask();

	bodyDriver->process();




}

void Ant::update()
{

	//pheromoneMap->addPheromone((int)bodyDriver->body.pos.x, (int)bodyDriver->body.pos.y, EXPLORED_PHEROMONE_ID, true, 1.0);
	


	/*
	if (sensorDriver->getPheromoneSenseFrontLeft(true) > sensorDriver->getPheromoneSenseFrontRight(true)) {
		bodyDriver->addDesiredMotion(Vector2(1, 1));

	}
	else {
		bodyDriver->addDesiredMotion(Vector2(1, -1));
	}*/

	actionDriver->doAction();
	taskDriver->doTask();
	pheromoneDriver->update();

	bodyDriver->update();


}

Body Ant::getBody()
{
	return bodyDriver->body;
}


