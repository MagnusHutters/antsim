#include "ActionExplore.h"

float ActionExplore::getStrenght()
{
	return strenght;
}

void ActionExplore::init()
{
	sensor->resetSensorResults();

	//sensor->setPrimarySensorPheromone(EXPLORED);

	//sensor->setPrimarySensorAngle(45);
	//sensor->enablePrimary();

	//sensor->enableDirection();
	//sensor->setDirectionSignumPositive(true);
	//sensor->setDirectionSensorPheromone(EXPLORED);
}

void ActionExplore::calcAction()
{
	//body->breakMotion();
	if (countdown <= 0) {

		direction = -sensor->senseDirection(exploredPheromone);
		countdown = rand() % 4 + 5;
		if (lookFor)
		{
			Vector2 lookFor = sensor->senseDirection(lookForPheromone);
			strenght = lookFor.Length();
			direction += lookFor;
		}
	}
	countdown--;
	//sensorLeftStrenght = sensor->sensePrimaryLeft(true);
	//sensorRightStrenght = sensor->sensePrimaryRight(true);
	//val3 = val1 - val2;


	//body->addDesiredMotion(Vector2(1, 1) * sensorLeftStrenght);
	//body->addDesiredMotion(Vector2(1, -1) * sensorRightStrenght);
	//body->
	body->addDesiredDirection(direction);

}
