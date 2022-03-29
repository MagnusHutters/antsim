#pragma once



#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"



#include "ActionDriver.h"

class ActionHold : public Action
{
public:


	ActionHold() 
	{
	
	}

	void calcAction() {
		body->breakMotion();
	}


};
