#pragma once



#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"



#include "ActionDriver.h"

class ActionExplore : public Action
{
public:



	ActionExplore(int PheromoneId) : PheromoneId(PheromoneId)
	{

	}

	void init() {
		sensor->resetSensorResults();

		//sensor->setPrimarySensorPheromone(EXPLORED);

		//sensor->setPrimarySensorAngle(45);
		//sensor->enablePrimary();

		//sensor->enableDirection();
		//sensor->setDirectionSignumPositive(true);
		//sensor->setDirectionSensorPheromone(EXPLORED);
	}

	void calcAction() {
		//body->breakMotion();
		if (countdown <= 0) {

			direction = sensor->senseDirection(PheromoneId,true);
			countdown = (rand() % 8) + 5;

		}
		countdown--;
		//sensorLeftStrenght = sensor->sensePrimaryLeft(true);
		//sensorRightStrenght = sensor->sensePrimaryRight(true);
		//val3 = val1 - val2;


		//body->addDesiredMotion(Vector2(1, 1) * sensorLeftStrenght);
		//body->addDesiredMotion(Vector2(1, -1) * sensorRightStrenght);
		//body->
		body->addDesiredDirection(-direction);

	}

private:
	int countdown = 0;
	int PheromoneId;
	Vector2 direction;

};
