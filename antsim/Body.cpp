#include "Body.h"
#include <stdlib.h>
#include "Config.h"
#include "ObstacleMap.h"

BodyDriver::BodyDriver(int x, int y, float rot, int id, MapContainer m) : id(id), toMove(0), toRotate(0), m(m)
{


	desiredDirection = Vector2::FromAngle(rot);
	velocity = desiredDirection;


	body = Body(static_cast<float>(x), static_cast<float>(y), rot);

	handle = m.bodyCollosionMap->registerEntity(body.pos);


}

void BodyDriver::breakMotion()
{
	//desiredDirection = -velocity;
	doBreak = true;
	desiredMotion = Vector2(0, 0);
}

void BodyDriver::addDesiredMotion(const Vector2& motion)
{
	if (motion!=motion) return;
	desiredMotion += motion;
}

void BodyDriver::addDesiredDirection(const Vector2& motion)
{
	if (motion != motion) return;
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
	toRotate += ((((float)rand() / (float)RAND_MAX) * 2.0f) - 1);
	toRotate = toRotate * 0.95f;


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

	desiredDirection = Vector2(0, 0);

	velocity = velocity + acceleration;
	velocity.ClampMagnitude(maxSpeed);

	
}

void BodyDriver::update()
{

	//body.pos += velocity;
	Vector2 newPos = body.pos + velocity;
	newPos= m.bodyCollosionMap->checkMovement(handle, body.pos, newPos);


	if(m.obstacleMap->isObstructed(newPos))
	{
		newPos = m.obstacleMap->getClosestUnobstructed(newPos,1);
	}
	if(newPos==Vector2(0,0))
	{
		newPos = Vector2::RandomWithinMap();
	}


	body.pos = newPos;



	m.bodyCollosionMap->move(handle, newPos);

	/*if (body.pos.x < 1)body.pos.x = 1;
	if (body.pos.y < 1)body.pos.y = 1;
	if (body.pos.x > 254)body.pos.x = 254;
	if (body.pos.y > 254)body.pos.y = 254;*/

	if (body.pos.x < 0)body.pos.x +=MAP_WIDTH;
	if (body.pos.y < 0)body.pos.y += MAP_HEIGHT;
	if (body.pos.x >= MAP_WIDTH)body.pos.x -= MAP_WIDTH;
	if (body.pos.y >= MAP_HEIGHT)body.pos.y -= MAP_HEIGHT;

	body.rot = velocity.GetAngle();


	if(pathCounter--<0)
	{
		
		pathStep();
		pathCounter += PATH_INTERVAL;
	}

}
