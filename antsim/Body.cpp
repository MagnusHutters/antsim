#include "Body.h"
#include <stdlib.h>

BodyDriver::BodyDriver(int x, int y, float rot)
{
	body = Body(x, y, rot);


	desiredDirection = Vector2::FromAngle(rot);
	velocity = desiredDirection;
	toRotate = 0;
	toMove = 0;


}

void BodyDriver::addDesiredMotion(const Vector2& motion)
{
	desiredMotion += motion;
}

void BodyDriver::addDesiredDirection(const Vector2& motion)
{
}

void BodyDriver::process()
{




	toMove = 1;
	toRotate += ((((float)rand() / (float)RAND_MAX) * 2.0) - 1);
	toRotate = toRotate * 0.95;


	desiredMotion.Rotate(body.rot);
	desiredDirection += desiredMotion;
	desiredMotion = Vector2(0, 0);


	desiredDirection = (desiredDirection + Vector2::RandomUnitVector() * wanderStrenght);

	Vector2 centerDirection = (Vector2(64, 64) - body.pos);
	centerDirection.Normalize();
	//desiredDirection += centerDirection * 0.05;

	desiredDirection.Normalize();
	Vector2 desiredVelocity = desiredDirection * maxSpeed;
	Vector2 steering = (desiredVelocity - velocity);
	Vector2 acceleration = steering;
	acceleration.ClampMagnitude(steerStrenght);
	velocity = velocity + acceleration;
	velocity.ClampMagnitude(maxSpeed);
}

void BodyDriver::update()
{

	body.pos += velocity;
	if (body.pos.x < 1)body.pos.x = 1;
	if (body.pos.y < 1)body.pos.y = 1;
	if (body.pos.x > 126)body.pos.x = 126;
	if (body.pos.y > 126)body.pos.y = 126;

	body.rot = velocity.GetAngle();
}
