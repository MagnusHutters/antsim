#pragma once



#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"



#include "ActionDriver.h"

class ActionFollowTrail : public Action
{
public:



	ActionFollowTrail(int PheromoneId) : PheromoneId(PheromoneId)
	{

	}

	void init() {
		sensor->resetSensor();

		sensor->setPrimarySensorAngle(45);
		sensor->enablePrimary();
	}

	void calcAction() {
		//body->breakMotion();


		sensorLeftStrenght = sensor->sensePrimaryLeft(true);
		sensorRightStrenght = sensor->sensePrimaryRight(true);
		//val3 = val1 - val2;


		body->addDesiredMotion(Vector2(1, 1) * sensorLeftStrenght);
		body->addDesiredMotion(Vector2(1, -1) * sensorRightStrenght);


	}

private:
	int PheromoneId;
	float sensorLeftStrenght = 0;
	float sensorRightStrenght = 0;
	float sensorCenterStrenght = 0;

};
