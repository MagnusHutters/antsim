#pragma once


#include "Vector2.h"
#include "Body.h"
#include "Pheromones2.h"
#include "EntityMap.h"
#include "Job.h"

#include "Config.h"



class SensorDriver
{
public:
	SensorDriver();
	SensorDriver(BodyDriver* body, PheromoneMap* pheromoneMap, EntityMap<Job>* jobMap);

	void doSense();

	
	void resetSensor();

	//===============GET AND SETTERS======================0
	inline void doGetComletePheromonePicture() { completePheromoneScan = true; }



	//PRIMARY SENSOR
	inline void setPrimarySensorPheromone(int id) { primaryPheromoneId = id; }
	inline void setPrimarySensorAngle(float angle);
	inline float sensePrimaryLeft(bool positive) {return primaryLeftPheromoneResult;}
	inline float sensePrimaryRight(bool positive) {return primaryRightPheromoneResult;}
	inline float sensePrimaryCenter(bool positive) { return primaryCenterPheromoneResult; }
	inline const PheromoneMapSensor& getPrimaryRight() { return getSensorFromVector(primaryRightVector); }
	inline const PheromoneMapSensor& getPrimaryLeft() { return getSensorFromVector(primaryLeftVector); }
	inline const PheromoneMapSensor& getPrimaryCenter() { return getSensorFromVector(primaryCenterVector); }
	inline void enablePrimary() { enabledPrimarySensor = true; }
	inline void disablePrimary() { enabledPrimarySensor = false; }

	//DIRECTION SENSOR
	inline void setDirectionSensorPheromone(int id) { directionPheromoneId = id; }
	inline Vector2 senseDirection() { return directionSensorResult; }
	inline void setDirectionSignumPositive(bool positive) { directionSensorSignum = positive; }
	inline void enableDirection() { enabledDirectionSensor = true; }
	inline void disableDirection() { enabledDirectionSensor = false; }


private:

	const PheromoneMapSensor& getSensorAtBody(int radius);
	const PheromoneMapSensor& getSensorFromVector(Vector2 vector);
	const PheromoneMapSensor& getSensorFromAngle(float angle);
	const Vector2& getSensorVectorFromAngle(float angle);

	BodyDriver* body;
	EntityMap<Job>* jobMap;
	PheromoneMap* pheromoneMap;


	

	
	//PRIMARY SENSOR
	bool enabledPrimarySensor = false;

	float primaryLeftPheromoneResult, primaryRightPheromoneResult, primaryCenterPheromoneResult;

	int primaryPheromoneId = 0;
	Vector2 primaryLeftVector, primaryRightVector, primaryCenterVector;

	const Vector2 frontLeft = Vector2(SENSOR_DISTANCE, SENSOR_DISTANCE);
	const Vector2 frontRight = Vector2(SENSOR_DISTANCE, -SENSOR_DISTANCE);
	const float sensorRadius = SENSOR_RADIUS;
	//Vector2 primaryLeftPheromone, primaryRightPheromone;




	//DIRECTION SENSOR
	bool enabledDirectionSensor = false;

	bool directionSensorSignum = true;
	Vector2 directionSensorResult;
	int directionPheromoneId = 0;



	//OTHER
	bool completePheromoneScan=false;
};

