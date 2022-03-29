#pragma once
#include "Vector2.h"


class CollisionMap;

#include "CollisionMap.h"

class Body
{

public:
	Vector2 pos;
	float32 rot;

	Body() {
		rot = 0;
		pos = Vector2(0,0);
	}

	Body(Vector2 pos, float32 rot) {
		this->pos = pos;
		this->rot = rot;
	}

	Body(float32 x, float32 y, float32 rot) {
		this->pos = Vector2(x,y);
		this->rot = rot;
	}
	

	Body(const Body& oldBody) {
		pos = oldBody.pos;
		rot = oldBody.rot;

	}
};

class BodyDriver {

public:


	BodyDriver();
	BodyDriver(int x, int y, float rot);


	void breakMotion();
	void addDesiredMotion(const Vector2& motion);
	void addDesiredDirection(const Vector2& motion);
	void setDesiredMotion(const Vector2& motion);
	void setDesiredDirection(const Vector2& motion);



	void process();
	void update();

	Body body;

private:



	Vector2 desiredMotion;

	bool doBreak = false;
	bool doWander = true;
	float maxSpeed = 2.0, steerStrenght = 0.3, wanderStrenght = 0.20;
	Vector2 velocity, desiredDirection;
	float toMove, toRotate;
};

