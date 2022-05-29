#include "ActionGoToPoint.h"

void ActionGoToPoint::calcAction()
{
	Vector2 currentPos = body->body.pos;

	
	//targetDirection.Normalize();
	if (!currentPath.exists) return;
	Vector2 waypoint = currentPath.tryAdvance(currentPos);
	

	Vector2 targetDirection = waypoint - currentPos;
	targetDirection.Normalize();
	body->addDesiredDirection(targetDirection);

	//goalInLineOfSightLast = goalInLineOfSight;

}
