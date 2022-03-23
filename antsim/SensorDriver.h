#pragma once


#include "Vector2.h"
#include "Body.h"
#include "Pheromones.h"


#define SENSOR_DISTANCE 3
#define SENSOR_RADIUS 4

class SensorDriver
{
public:
	SensorDriver();
	SensorDriver(BodyDriver* body, PheromoneMap* pheromoneMap);

	void doSense();




	inline void doGetComletePheromonePicture() { completePheromoneScan = true; }
	inline void setSensePheromone(int id) { pheromoneId = id; }
	inline float getPheromoneSenseFrontLeft(bool positive) {return frontLeftPheromone[positive];}
	inline float getPheromoneSenseFrontRight(bool positive) {return frontRightPheromone[positive];}

	const PheromoneMapSensor& getSensorFromVector(Vector2 vector);



private:



	BodyDriver* body;

	const Vector2 frontLeft = Vector2(SENSOR_DISTANCE, SENSOR_DISTANCE);
	const Vector2 frontRight = Vector2(SENSOR_DISTANCE, -SENSOR_DISTANCE);
	const float sensorRadius = SENSOR_RADIUS;

	int pheromoneId = 0;
	

	Vector2 frontLeftPheromone, frontRightPheromone;
	bool completePheromoneScan=false;


	PheromoneMap* pheromoneMap;

};

