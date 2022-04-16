#pragma once



#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"



#include "ActionDriver.h"

class ActionGoTowardNearbyTask : public Action
{
public:



	ActionGoTowardNearbyTask()
	{

	}

	void init() {
		sensor->resetSensorResults();

	}

	void calcAction() {
		Vector2 absDir = sensor->senseJob();
		body->addDesiredDirection(absDir);



	}

private:

};
