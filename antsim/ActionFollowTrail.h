#pragma once



#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"



#include "ActionDriver.h"
#include "PheromoneId.h"

class ActionFollowTrail : public Action
{
public:



	ActionFollowTrail(PheromoneId PheromoneId) : pheromoneId(PheromoneId)
	{

	}

	void init() {
		sensor->resetSensorResults();

		//sensor->setPrimarySensorPheromone(EXPLORED);
		//sensor->setPrimarySensorAngle(45);
		//sensor->enablePrimary();
	}

	void calcAction() {
		//body->breakMotion();

		sensorCenterStrenght = sensor->sensePrimaryCenter(pheromoneId, 45);
		sensorLeftStrenght = sensor->sensePrimaryLeft(pheromoneId, 45);
		sensorRightStrenght = sensor->sensePrimaryRight(pheromoneId, 45);
		//val3 = val1 - val2;
		float totalValue = sensorCenterStrenght + sensorLeftStrenght + sensorRightStrenght;
		float valueUnder = sensor->senseBelow(pheromoneId, SENSOR_RADIUS_SMALL);
		if(totalValue < PHEROMONE_FLOOR || valueUnder < PHEROMONE_FLOOR)
		{
			lostCounter += 1;
			if (lostCounter > 10)lostCounter = 10;
		}else
		{
			lostCounter -= 1;
			if (lostCounter < 0)lostCounter = 0;
		}

		if(lostCounter>5)
		{
			Vector2 dir = sensor->senseDirection(pheromoneId);
			body->addDesiredDirection(dir);
		}else
		{
			body->addDesiredMotion(Vector2(1, 1) * sensorLeftStrenght);
			body->addDesiredMotion(Vector2(1, -1) * sensorRightStrenght);
		}




	}

private:
	int lostCounter = 10;
	PheromoneId pheromoneId;
	float sensorLeftStrenght = 0;
	float sensorRightStrenght = 0;
	float sensorCenterStrenght = 0;
	//float total value

};
