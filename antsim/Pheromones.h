/*
#pragma once
#include <vector>
#include <cstdint>
#include <cmath> 
#include <algorithm> 
#include "Vector2.h"
#include <mutex>
#include <math.h>
#include "Config.h"








const struct PheromoneMapSensor {

	float x, y, radius, radius2;
	Vector2 vector;

	PheromoneMapSensor() {
		x = 0; y = 0; radius = 0; radius2 = 0;

	}
	PheromoneMapSensor(float x, float y, float radius) : x(x), y(y), radius(radius), radius2(radius * radius), vector(Vector2(x,y)) {

	}
	PheromoneMapSensor(Vector2 vector, float radius) : x(vector.x), y(vector.y), radius(radius), radius2(radius* radius), vector(vector) {

	}
	PheromoneMapSensor setX(float newX) { return PheromoneMapSensor(newX, y, radius); }
	PheromoneMapSensor setY(float newY) { return PheromoneMapSensor(x, newY, radius); }
	PheromoneMapSensor setPos(float newX, float newY) { return PheromoneMapSensor(newX, newY, radius); }
	PheromoneMapSensor setPos(Vector2 vector) { return PheromoneMapSensor(vector, radius); }
	PheromoneMapSensor setRadius(float radius) { return PheromoneMapSensor(x,y, radius); }


};

struct PheromoneMapParams {
	int x;
	int y;
	uint32_t bitMap;
	int id;
	bool positive;
	float strenght;
	PheromoneMapSensor sensor;
	Vector2 origin;

	PheromoneMapParams(int x, int y, uint32_t bitMap, int id, bool positive, float strenght) : 
		x(x), 
		y(y), 
		bitMap(bitMap), 
		id(id), 
		positive(positive), 
		strenght(strenght), 
		sensor(PheromoneMapSensor()), 
		origin(Vector2())
	{}
	

	PheromoneMapParams(uint32_t bitMap, int id, bool positive) : 
		x(0), 
		y(0), 
		bitMap(bitMap), 
		id(id), 
		positive(positive), 
		strenght(0),
		sensor(PheromoneMapSensor()),
		origin(Vector2())	 
	{}

	PheromoneMapParams(const Vector2& origin, const PheromoneMapSensor& sensor, uint32_t bitMap, int id, bool positive) :
		x(0),
		y(0),
		bitMap(bitMap),
		id(id),
		positive(positive),
		strenght(0),
		sensor(sensor),
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
	

	friend class MiddlePheromoneMap;
	friend class InnerPheromoneMap;
	friend class PheromoneMap;

	BasePheromoneMap* outer;



	BasePheromoneMap* children[4];

	virtual void registerInner(BasePheromoneMap* inner, int x, int y) {

	}

	virtual float getPheromone(int id, bool positive) {
		return 0.0;
	}


	float squareDistanceToPoint(Vector2 point){ 
		float x_min = xpos;
		float y_min = ypos;
		float x_max = x_min + size;
		float y_max = y_min + size;
		

		if (point.x < x_min) {
			if (point.y < y_min)
				return pow(x_min - point.x,2)+ pow(y_min - point.y,2);
			else if (point.y <= y_max)
				return pow(x_min - point.x,2);
			else
				return pow(x_min - point.x, 2) + pow(y_max - point.y, 2);

		}
		else if (point.x <= x_max) {
			if (point.y < y_min)
				return y_min - point.y;
			else if (point.y <= y_max)
				return 0;
			else
				return point.y - y_max;
		}
		else {
			if (point.y < y_min)
				return pow(x_max - point.x,2) + (y_min - point.y,2);
			else if (point.y <= y_max)
				return pow(point.x - x_max,2);
			else
				return pow(x_max - point.x,2) + (y_max - point.y,2);
		}
	}

	bool sensorInRange(const PheromoneMapSensor &sensor) {

		float dist2 = squareDistanceToPoint(sensor.vector);
		if(dist2< sensor.radius2) return true;
		return false;
	}

private:

	virtual float _getPheromoneStrenght(PheromoneMapParams* p);

	virtual Vector2 _getPheromoneStrenghtDual(PheromoneMapParams* p);

	virtual Vector2 _getPheromoneVector(PheromoneMapParams* p);

	virtual void _setPheromone(PheromoneMapParams* p);

	virtual void _addPheromone(PheromoneMapParams* p);

	virtual bool _decayPheromone(PheromoneMapParams* p);

	

};


class InnerPheromoneMap : public BasePheromoneMap {

public:

	//std::vector<std::vector<float>>pheromones;

	InnerPheromoneMap();

	



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

	float getPheromone(int id, bool positive) {
		return pheromones[id][positive];
	}

private:

	Vector2 _getPheromoneVector(PheromoneMapParams* p);

	float _getPheromoneStrenght(PheromoneMapParams* p);
	
	Vector2 _getPheromoneStrenghtDual(PheromoneMapParams* p);

	void _addPheromone(PheromoneMapParams* p);

	void _setPheromone(PheromoneMapParams* p);

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

private:
	
	void _addPheromone(PheromoneMapParams* p);
	void _setPheromone(PheromoneMapParams* p);
	//bool _decayPheromone(PheromoneMapParams* p);

};

class PheromoneMap : public BasePheromoneMap {
public:

	std::vector<std::vector<BasePheromoneMap*>> innerPheromones;


	PheromoneMap(int sizeX, int sizeY);


	Vector2& sensePheromonesVector(const Vector2& origin, const PheromoneMapSensor& sensor, int id, bool positive) {

		uint32_t bitMap = (uint32_t)1 << id;
		PheromoneMapParams* params = new PheromoneMapParams(origin, sensor, bitMap, id, positive);

		Vector2 vector = _getPheromoneVector(params);

		delete(params);

		return vector;
	}

	float sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id, bool positive);

	Vector2 sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id);

	void registerInner(BasePheromoneMap* inner, int x, int y);

	void doDecayPheromones();
	void addPheromone(int x, int y, int id, bool positive, float strenght);

private:

	void _setPheromone(PheromoneMapParams* p);
};




*/