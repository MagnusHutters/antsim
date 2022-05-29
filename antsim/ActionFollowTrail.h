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

	float getStrenght()
	{
		return (float)lostCounter;
	}

	void init() {
		sensor->resetSensorResults();

		//sensor->setPrimarySensorPheromone(EXPLORED);
		//sensor->setPrimarySensorAngle(45);
		//sensor->enablePrimary();
	}

	void calcAction() {
		//body->breakMotion();

		centerStrenght = sensor->sensePrimaryCenter(pheromoneId, angle);
		leftStrenght = sensor->sensePrimaryLeft(pheromoneId, angle);
		rightStrenght = sensor->sensePrimaryRight(pheromoneId, angle);

		if(centerStrenght>= std::max(leftStrenght, rightStrenght)*2)
		{
			angle -= 5;
			if (angle < 15)
			{
				angle = 15;
			}
		}else
		{
			angle += 10;
			if (angle > 65)
			{
				angle = 65;
			}
			
		}



		float totalStrength = centerStrenght + leftStrenght + rightStrenght;
		if(totalStrength>0)
		{
			centerStrenght /= totalStrength;
			leftStrenght /= totalStrength;
			rightStrenght /= totalStrength;
		}

		//val3 = val1 - val2;
		float totalValue = centerStrenght + leftStrenght + rightStrenght;
		float valueUnder = sensor->senseBelow(pheromoneId, SENSOR_RADIUS_LARGE);
		//lostCounter = 20;
		if(totalValue < PHEROMONE_FLOOR || valueUnder < PHEROMONE_FLOOR)
		{
			
			lostCounter += 1;
			
		}else
		{
			if (lostCounter > 30)lostCounter = 30;
			lostCounter -= 2;
			if (lostCounter < 0)lostCounter = 0;
		}

		if(lostCounter>15)
		{
			Vector2 dir = sensor->senseDirection(pheromoneId);
			body->addDesiredDirection(dir);
		}else
		{

			body->addDesiredMotion(Vector2(3, 3) * leftStrenght);
			body->addDesiredMotion(Vector2(3, 0) * centerStrenght);
			body->addDesiredMotion(Vector2(3, -3) * rightStrenght);
		}

		Vector2 obstruction = sensor->senseObstructions()*0.6f;

		obstruction.Rotate(90);
		body->addDesiredDirection(obstruction);

		Vector2 terrain = sensor->senseTerrain();
		body->addDesiredDirection(-terrain*5);

	}

private:
	int lostCounter = 30;

	int angle = 45;
	PheromoneId pheromoneId;
	float leftStrenght = 0;
	float rightStrenght = 0;
	float centerStrenght = 0;
	//float total value

};
