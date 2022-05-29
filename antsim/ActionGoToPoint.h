#pragma once
#include "ActionDriver.h"
#include "Pathfinder.h"

class ActionGoToPoint :
    public Action
{
public:

	ActionGoToPoint(Vector2 target) : targetPos(target)
	{

	}
	

	//float getStrenght();

	void init()
	{
		currentPath = Pathfinder(sensor->getObstacleMap(), targetPos, body->body.pos);
	}

	void calcAction();

private:


	Pathfinder currentPath = Pathfinder();
	Vector2 targetPos;
	bool goalInLineOfSightLast = true;
};

