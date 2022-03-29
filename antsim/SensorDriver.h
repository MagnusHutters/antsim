#pragma once


#include "Vector2.h"
#include "Body.h"
#include "Pheromones2.h"

#include "Config.h"



class SensorDriver
{
public:
	SensorDriver();
	SensorDriver(BodyDriver* body, PheromoneMap* pheromoneMap);

	void doSense();

	
	void resetSensor();

	//===============GET AND SETTERS======================0
	inline void doGetComletePheromonePicture() { completePheromoneScan = true; }
	inline void setSensePheromone(int id) { pheromoneId = id; }

	inline void setPrimarySensorAngle(float angle);


	//PRIMARY SENSOR
	inline float sensePrimaryLeft(bool positive) {return primaryLeftPheromone;}
	inline float sensePrimaryRight(bool positive) {return primaryRightPheromone;}
	inline float sensePrimaryCenter(bool positive) { return primaryCenterPheromone; }
	inline const PheromoneMapSensor& getPrimaryRight() { return getSensorFromVector(primaryRightVector); }
	inline const PheromoneMapSensor& getPrimaryLeft() { return getSensorFromVector(primaryLeftVector); }
	inline const PheromoneMapSensor& getPrimaryCenter() { return getSensorFromVector(primaryCenterVector); }
	inline void enablePrimary() { enabledPrimarySensor = true; }
	inline void disablePrimary() { enabledPrimarySensor = false; }


private:

	const PheromoneMapSensor& getSensorFromVector(Vector2 vector);
	const PheromoneMapSensor& getSensorFromAngle(float angle);
	const Vector2& getSensorVectorFromAngle(float angle);

	BodyDriver* body;

	const Vector2 frontLeft = Vector2(SENSOR_DISTANCE, SENSOR_DISTANCE);
	const Vector2 frontRight = Vector2(SENSOR_DISTANCE, -SENSOR_DISTANCE);
	const float sensorRadius = SENSOR_RADIUS;

	int pheromoneId = 0;
	

	float primaryLeftPheromone, primaryRightPheromone, primaryCenterPheromone;
	Vector2 primaryLeftVector, primaryRightVector, primaryCenterVector;
	//Vector2 primaryLeftPheromone, primaryRightPheromone;


	bool completePheromoneScan=false;
	bool enabledPrimarySensor = false;


	PheromoneMap* pheromoneMap;

};

