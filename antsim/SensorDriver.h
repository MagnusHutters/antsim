#pragma once


#include "Vector2.h"
#include "Body.h"
#include "Pheromones2.h"
#include "JobMap.h"
#include "Job.h"

#include "Config.h"
#include "ObstacleMap.h"


class SensorDriver
{
public:
	SensorDriver();
	SensorDriver(BodyDriver* body, MapContainer m);

	void doSense();

	
	void resetSensorResults();

	//===============GET AND SETTERS======================0
	//inline void doGetComletePheromonePicture() { completePheromoneScan = true; }


	float sensePrimaryLeft(PheromoneId id, float angle) {

		return m.pheromoneMap->sensePheromonesStrenght(getSensorFromAngle(angle), id.id, id.positive);
	}
	float sensePrimaryRight(PheromoneId id, float angle) {

		return m.pheromoneMap->sensePheromonesStrenght(getSensorFromAngle(-angle), id.id, id.positive);
	}
	float sensePrimaryCenter(PheromoneId id, float angle) {

		return m.pheromoneMap->sensePheromonesStrenght(getSensorFromAngle(0), id.id, id.positive);
	}

	float senseBelow(const PheromoneId& pheromone, int radius = SENSOR_RADIUS_MEDIUM)
	{
		return m.pheromoneMap->sensePheromonesStrenght(getSensorAtBody(radius), pheromone.id, pheromone.positive);
	}


	inline PheromoneMapSensor getPrimaryRight() { return getSensorFromVector(primaryRightVector); }
	inline PheromoneMapSensor getPrimaryLeft() { return getSensorFromVector(primaryLeftVector); }
	inline PheromoneMapSensor getPrimaryCenter() { return getSensorFromVector(primaryCenterVector); }

	Vector2 senseDirection(PheromoneId pheromone, float obstructionResponse=0);
	float senseStrenght(PheromoneId pheromone);

	Vector2 senseObstructions();
	Vector2 senseTerrain();


	Vector2 senseJob(const Conditions& conditions = Conditions()) {
		auto entity = m.jobMap->getClosest(body->body.pos, conditions);
		if(entity.success)
		{
			const float distance = entity.entity->pos.Distance(body->body.pos);
			if (distance <= JOB_SENSOR_RADIUS) {
				return entity.entity->pos - body->body.pos;
			
			}
		}
		return Vector2(0, 0);
	}

	ObstacleMap* getObstacleMap()
	{
		return m.obstacleMap;
	}

	struct _getJobResult { bool success; Job* entity; };
	_getJobResult getJobFromHandle(int jobHandle)
	{
		auto allJobs = m.jobMap->getEntities();
		auto it = allJobs.find(jobHandle);
		if (it == allJobs.end()) return { false, nullptr };

		return { true, it->second };
	}

	_getJobResult getJobPheromone(PheromoneId pheromone)
	{
		auto allJobs = m.jobMap->getEntities();
		for (auto job : allJobs)
		{
			auto interfaceResult = job.second->getInterface(pheromone);
			if(interfaceResult.sucess)
			{
				return { true,job.second };
			}
		}

		return { false, nullptr };
	}

	_getJobResult getJob(const Conditions& conditions=Conditions()) {
		auto entity = m.jobMap->getClosest(body->body.pos, conditions);

		if (entity.success)
		{
			const float distance = entity.entity->pos.Distance(body->body.pos);
			if (distance <= JOB_INTERACT_RADIUS) {
				return { entity.success,entity.entity };

			}
		}
		return {false,entity.entity};
	}

	bool checkLineOfSight(Vector2 p1, Vector2 p2)
	{
		return m.obstacleMap->checkLineOfSight(p1, p2);
	}

private:

	const PheromoneMapSensor getSensorAtBody(int radius);
	const PheromoneMapSensor getSensorFromVector(Vector2 vector);
	const PheromoneMapSensor getSensorFromAngle(float angle);
	const Vector2 getSensorVectorFromAngle(float angle);

	BodyDriver* body;
	MapContainer m;


	

	
	//PRIMARY SENSOR
	bool enabledPrimarySensor = false;

	float primaryLeftPheromoneResult, primaryRightPheromoneResult, primaryCenterPheromoneResult;

	int primaryPheromoneId = 0;
	Vector2 primaryLeftVector, primaryRightVector, primaryCenterVector;

	const Vector2 frontLeft = Vector2(SENSOR_DISTANCE, SENSOR_DISTANCE);
	const Vector2 frontRight = Vector2(SENSOR_DISTANCE, -SENSOR_DISTANCE);
	const float sensorRadius = SENSOR_RADIUS_MEDIUM;
	//Vector2 primaryLeftPheromone, primaryRightPheromone;




	//DIRECTION SENSOR
	bool enabledDirectionSensor = false;

	bool directionSensorSignum = true;
	Vector2 directionSensorResult;
	int directionPheromoneId = 0;



	//OTHER
	bool completePheromoneScan=false;
};

