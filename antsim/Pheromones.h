#pragma once
#include <vector>
#include <cstdint>
#include <algorithm> 
#include "Vector2.h"
#include <mutex>




#define MAX_NUMBER_OF_TASKS				16
#define DEFAULT_PHEROMONES_NUMBER		4
#define NUMBER_OF_PHEROMONE_PAIRS		MAX_NUMBER_OF_TASKS+DEFAULT_PHEROMONES_NUMBER
#define EXPLORED						MAX_NUMBER_OF_TASKS+0
#define PATH							MAX_NUMBER_OF_TASKS+1
//#define OUTER_CONTAINS_MAP_RESOLUTION	32

#define PHEROMONE_FLOOR					0.5
#define PHEROMONE_DECAY					0.995






const struct PheromoneSensor {

	float x, y, radius, radius2;
	Vector2 vector;

	PheromoneSensor() {
		x = 0; y = 0; radius = 0; radius2 = 0;

	}
	PheromoneSensor(float x, float y, float radius) : x(x), y(y), radius(radius), radius2(radius * radius), vector(Vector2(x,y)) {

	}
	PheromoneSensor(Vector2 vector, float radius) : x(vector.x), y(vector.y), radius(radius), radius2(radius* radius), vector(vector) {

	}
	PheromoneSensor setX(float newX) { return PheromoneSensor(newX, y, radius); }
	PheromoneSensor setY(float newY) { return PheromoneSensor(x, newY, radius); }
	PheromoneSensor setPos(float newX, float newY) { return PheromoneSensor(newX, newY, radius); }
	PheromoneSensor setPos(Vector2 vector) { return PheromoneSensor(vector, radius); }
	PheromoneSensor setRadius(float radius) { return PheromoneSensor(x,y, radius); }


};

struct PheromoneMapParams {
	int x;
	int y;
	uint32_t bitMap;
	int id;
	bool positive;
	float strenght;
	PheromoneSensor sensor;
	Vector2 origin;

	PheromoneMapParams(int x, int y, uint32_t bitMap, int id, bool positive, float strenght) : 
		x(x), 
		y(y), 
		bitMap(bitMap), 
		id(id), 
		positive(positive), 
		strenght(strenght), 
		sensor(PheromoneSensor()), 
		origin(Vector2())
	{}
	

	PheromoneMapParams(uint32_t bitMap, int id, bool positive) : 
		x(0), 
		y(0), 
		bitMap(bitMap), 
		id(id), 
		positive(positive), 
		strenght(0),
		sensor(PheromoneSensor()),
		origin(Vector2())	 
	{}

	PheromoneMapParams(const Vector2& origin, const PheromoneSensor& sensor, uint32_t bitMap, int id, bool positive) :
		x(0),
		y(0),
		bitMap(bitMap),
		id(id),
		positive(positive),
		strenght(0),
		sensor(PheromoneSensor()),
		origin(Vector2())
	{}

};


class BasePheromoneMap {
public:

	//float pheromones[NUMBER_OF_PHEROMONE_PAIRS][2];

	
	uint32_t contains[2]; //[1]: positive, [0] negative
	int size;
	int xpos;
	int ypos;
	
	BasePheromoneMap* outer;

	BasePheromoneMap* children[4];


	bool sensorInRange(const PheromoneSensor &sensor) {
		if ((sensor.x > xpos) && (sensor.x < (xpos + size)) && (sensor.y > ypos) && (sensor.y < (ypos + size))) return true;
		if( ((sensor.x - (xpos			)) * (sensor.x - (xpos			))) + ((sensor.y - (ypos		)) * (sensor.y - (ypos			))) < sensor.radius2) return true;
		if (((sensor.x - (xpos + size	)) * (sensor.x - (xpos + size	))) + ((sensor.y - (ypos		)) * (sensor.y - (ypos			))) < sensor.radius2) return true;
		if (((sensor.x - (xpos			)) * (sensor.x - (xpos			))) + ((sensor.y - (ypos + size	)) * (sensor.y - (ypos + size	))) < sensor.radius2) return true;
		if (((sensor.x - (xpos + size	)) * (sensor.x - (xpos + size	))) + ((sensor.y - (ypos + size	)) * (sensor.y - (ypos + size	))) < sensor.radius2) return true;
		return false;
	}



	virtual float _getPheromoneStrenght(PheromoneMapParams* p) {
		if ((contains[p->positive] & p->bitMap) == false) return 0.0f;
		if (sensorInRange(p->sensor)) {
			return
				children[0]->_getPheromoneStrenght(p) +
				children[1]->_getPheromoneStrenght(p) +
				children[2]->_getPheromoneStrenght(p) +
				children[3]->_getPheromoneStrenght(p);
		}
		return 0.0f;
	}

	virtual Vector2 _getPheromoneVector(PheromoneMapParams* p) {
		if ((contains[p->positive] & p->bitMap) == false) return Vector2();
		if (sensorInRange(p->sensor)) {
			return
				children[0]->_getPheromoneVector(p) +
				children[1]->_getPheromoneVector(p) +
				children[2]->_getPheromoneVector(p) +
				children[3]->_getPheromoneVector(p);
		}
		return Vector2();
	}


	virtual void _addPheromone(PheromoneMapParams* p);

	virtual bool _decayPheromone(PheromoneMapParams* p);

	virtual void registerInner(BasePheromoneMap* inner, int x, int y) {

	}

};


class InnerPheromoneMap : public BasePheromoneMap {

public:

	//std::vector<std::vector<float>>pheromones;

	InnerPheromoneMap();


	Vector2 _getPheromoneVector(PheromoneMapParams* p) {
		locker.lock();
		if ((contains[p->positive] & p->bitMap) == false) return Vector2();
		
		Vector2 center = Vector2((float)xpos + 0.5, (float)xpos + 0.5);
		float dist = center.DistanceSquared(p->sensor.vector);
		if (dist < p->sensor.radius2) {

			Vector2 value= (center - p->origin) * pheromones[p->id][p->positive];
			locker.unlock();
			return value;
		}
		locker.unlock();
		return Vector2();
	}

	float _getPheromoneStrenght(PheromoneMapParams* p) {
		locker.lock();
		Vector2 center = Vector2((float)xpos + 0.5, (float)xpos + 0.5);
		float dist = center.DistanceSquared(p->sensor.vector);
		if (dist < p->sensor.radius2) {
			float value= pheromones[p->id][p->positive];
			locker.unlock();
			return value;
		}
		locker.unlock();
		return 0;
	}



	bool getIsChanged() {
		locker.lock();


		return changed;
		locker.unlock();
	}
	std::vector<std::vector<float>> getPheromones() {
		locker.lock();


		locker.unlock();
	}



	InnerPheromoneMap(int size, int xpos, int ypos, BasePheromoneMap* outer);

	void _addPheromone(PheromoneMapParams* p);

	bool _decayPheromone(PheromoneMapParams* p);

private:
	std::vector<std::vector<float>> pheromones;
	bool changed = false;

	std::mutex locker;
};

class MiddlePheromoneMap : public BasePheromoneMap{
public:



	//Constructor
	MiddlePheromoneMap();
	MiddlePheromoneMap(int size, int xpos, int ypos, BasePheromoneMap* outer);

	
	void _addPheromone(PheromoneMapParams* p);

	//bool _decayPheromone(PheromoneMapParams* p);

};

class PheromoneMap : public BasePheromoneMap {
public:

	std::vector<std::vector<BasePheromoneMap*>> innerPheromones;


	PheromoneMap(int sizeX, int sizeY);


	Vector2& sensePheromonesVector(const Vector2& origin, const PheromoneSensor& sensor, int id, bool positive) {

		uint32_t bitMap = (uint32_t)1 << id;
		PheromoneMapParams* params = new PheromoneMapParams(origin, sensor, bitMap, id, positive);

		Vector2 vector = _getPheromoneVector(params);

		delete(params);

		return vector;
	}

	float sensePheromonesStrenght(const PheromoneSensor& sensor, int id, bool positive) {
		uint32_t bitMap = (uint32_t)1 << id;
		PheromoneMapParams* params = new PheromoneMapParams(Vector2(), sensor, bitMap, id, positive);

		float strenght = _getPheromoneStrenght(params);

		delete(params);

		return strenght;
	}
	void registerInner(BasePheromoneMap* inner, int x, int y);

	void doDecayPheromones();
	void addPheromone(int x, int y, int id, bool positive, float strenght);
};




