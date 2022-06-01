#pragma once
#include "MapContainer.h"
#include "Packet.h"
#include "PathArchive.h"
#include "RigidBodyMap.h"
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
	BodyDriver(int x, int y, float rot, int id, MapContainer m);


	void breakMotion();
	void addDesiredMotion(const Vector2& motion);
	void addDesiredDirection(const Vector2& motion);
	void setDesiredMotion(const Vector2& motion);
	void setDesiredDirection(const Vector2& motion);

	Vector2 getGlobalVector(Vector2 localVector)
	{
		return localVector.Rotate(body.rot);
	}


	void startPath(int handle)
	{

		m.pathArchive->startPath(id, packet->getTypeId(),body.pos, handle);
		onPath = true;
	}
	void endPath()
	{
		m.pathArchive->endPath(id,body.pos);
		onPath = false;
	}
	void pathStep()
	{
		if(onPath)
		{
			m.pathArchive->stepPath(id, body.pos);
		}
	}


	void process();
	void update();
	int getId() { return id; }

	Body body;

	Packet* packet;
	bool hasPacket=false;
	MapContainer m;

	int handle;

private:
	friend class Logger;

	int id;

	Vector2 desiredMotion;

	bool doBreak = false;
	bool doWander = true;
	float maxSpeed = 1.5f, steerStrenght = 0.4f, wanderStrenght = 0.05f;
	Vector2 velocity, desiredDirection;
	float toMove, toRotate;

	int pathCounter = 0;
	bool onPath;
	
};

