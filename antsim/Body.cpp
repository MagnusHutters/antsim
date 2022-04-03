#include "Body.h"
#include <stdlib.h>
#include "Config.h"

BodyDriver::BodyDriver(int x, int y, float rot)
{
	body = Body(x, y, rot);


	desiredDirection = Vector2::FromAngle(rot);
	velocity = desiredDirection;
	toRotate = 0;
	toMove = 0;


}

void BodyDriver::breakMotion()
{
	//desiredDirection = -velocity;
	doBreak = true;
	desiredMotion = Vector2(0, 0);
}

void BodyDriver::addDesiredMotion(const Vector2& motion)
{
	desiredMotion += motion;
}

void BodyDriver::addDesiredDirection(const Vector2& motion)
{
	desiredDirection += motion;
}

void BodyDriver::process()
{

	if (doBreak) {
		velocity *= 0.5;
		doBreak = false;
		desiredMotion = Vector2(0, 0);
		return;
	}


	toMove = 1;
	toRotate += ((((float)rand() / (float)RAND_MAX) * 2.0) - 1);
	toRotate = toRotate * 0.95;


	desiredMotion.Rotate(body.rot);
	desiredDirection += desiredMotion;


	desiredMotion = Vector2(0, 0);

	desiredDirection.Normalize();
	if (doWander) {
		desiredDirection = (desiredDirection * (1 - wanderStrenght) + Vector2::RandomUnitVector() * wanderStrenght);
	}

	//Vector2 centerDirection = (Vector2(128, 128) - body.pos);
	//centerDirection.Normalize();
	//desiredDirection += centerDirection * 0.0;

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
	
	/*if (body.pos.x < 1)body.pos.x = 1;
	if (body.pos.y < 1)body.pos.y = 1;
	if (body.pos.x > 254)body.pos.x = 254;
	if (body.pos.y > 254)body.pos.y = 254;*/

	if (body.pos.x < 0)body.pos.x +=MAP_WIDTH;
	if (body.pos.y < 0)body.pos.y += MAP_HEIGHT;
	if (body.pos.x >= MAP_WIDTH)body.pos.x -= MAP_WIDTH;
	if (body.pos.y >= MAP_HEIGHT)body.pos.y -= MAP_HEIGHT;

	body.rot = velocity.GetAngle();
}
