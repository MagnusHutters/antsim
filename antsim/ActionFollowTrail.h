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
		sensor->resetSensorResults();

		//sensor->setPrimarySensorPheromone(EXPLORED);
		//sensor->setPrimarySensorAngle(45);
		//sensor->enablePrimary();
	}

	void calcAction() {
		//body->breakMotion();

		sensorCenterStrenght = sensor->sensePrimaryCenter(PheromoneId, 45, true);
		sensorLeftStrenght = sensor->sensePrimaryLeft(PheromoneId, 45, true);
		sensorRightStrenght = sensor->sensePrimaryRight(PheromoneId, 45, true);
		//val3 = val1 - val2;
		float totalValue = sensorCenterStrenght + sensorLeftStrenght + sensorRightStrenght;
		float valueUnder = sensor->senseBelow(PheromoneId, true, SENSOR_RADIUS_SMALL);
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
			Vector2 dir = sensor->senseDirection(PheromoneId, true);
			body->addDesiredDirection(dir);
		}else
		{
			body->addDesiredMotion(Vector2(1, 1) * sensorLeftStrenght);
			body->addDesiredMotion(Vector2(1, -1) * sensorRightStrenght);
		}




	}

private:
	int lostCounter = 10;
	int PheromoneId;
	float sensorLeftStrenght = 0;
	float sensorRightStrenght = 0;
	float sensorCenterStrenght = 0;
	//float total value

};
