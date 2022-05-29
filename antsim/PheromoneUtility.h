#pragma once
#include <list>

#include "Vector2.h"


const struct PheromoneMapSensor {

	float x, y, radius, radius2;
	Vector2 vector;

	PheromoneMapSensor() {
		x = 0; y = 0; radius = 0; radius2 = 0;

	}
	PheromoneMapSensor(float x, float y, float radius) : x(x), y(y), radius(radius), radius2(radius* radius), vector(Vector2(x, y)) {

	}
	PheromoneMapSensor(Vector2 vector, float radius) : x(vector.x), y(vector.y), radius(radius), radius2(radius* radius), vector(vector) {

	}
	PheromoneMapSensor setX(float newX) { return PheromoneMapSensor(newX, y, radius); }
	PheromoneMapSensor setY(float newY) { return PheromoneMapSensor(x, newY, radius); }
	PheromoneMapSensor setPos(float newX, float newY) { return PheromoneMapSensor(newX, newY, radius); }
	PheromoneMapSensor setPos(Vector2 vector) { return PheromoneMapSensor(vector, radius); }
	PheromoneMapSensor setRadius(float radius) { return PheromoneMapSensor(x, y, radius); }


};

struct IntCoords {
	int x, y;
	bool operator==(const IntCoords& intCoords) const
	{
		return x == intCoords.x && y == intCoords.y;
	}
};



class ActivePheromoneInfo
{
public:
	int x, y, id, positive;
	float* value;
	bool* active;
	unsigned long lastDecayTick;


};


struct PheromoneActiveReturn
{
	
	//std::list<ActivePheromoneInfo>* active;
	std::list<ActivePheromoneInfo>* toUpdate;
	std::list<ActivePheromoneInfo> clearedCells;
};



