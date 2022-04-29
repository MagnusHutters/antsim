#include "SensorDriver.h"

SensorDriver::SensorDriver(BodyDriver* body, PheromoneMap* pheromoneMap, EntityMap<Job>* jobMap) : body(body), pheromoneMap(pheromoneMap), jobMap(jobMap)
{
	resetSensorResults();

}

void SensorDriver::doSense()
{
	
	if (enabledPrimarySensor) {

	}
	if (enabledDirectionSensor) {
		}


}

void SensorDriver::resetSensorResults()
{
	//setPrimarySensorAngle(45);

	primaryPheromoneId = -1;
	directionPheromoneId = -1;

	enabledDirectionSensor = false;
	enabledPrimarySensor = false;
}

//inline void SensorDriver::setPrimarySensorAngle(float angle)
//{
//	primaryCenterVector = getSensorVectorFromAngle(0);
//	primaryLeftVector = getSensorVectorFromAngle(angle);
//	primaryRightVector = getSensorVectorFromAngle(-angle);
//}



Vector2 SensorDriver::senseDirection(PheromoneId pheromone) { 
	return pheromoneMap->sensePheromonesStrenghtDirection(getSensorAtBody(SENSOR_RADIUS_HUGE), body->body.pos, pheromone.id, pheromone.positive);
 
}

float SensorDriver::senseStrenght(PheromoneId pheromone)
{
	return pheromoneMap->sensePheromonesStrenght(getSensorAtBody(SENSOR_RADIUS_HUGE), pheromone.id, pheromone.positive);
	
}

inline const PheromoneMapSensor SensorDriver::getSensorAtBody(int radius)
{
	return PheromoneMapSensor(body->body.pos, radius);
}

inline const PheromoneMapSensor SensorDriver::getSensorFromVector(Vector2 vector)
{
	

	vector.Rotate(body->body.rot);

	vector += body->body.pos;

	return PheromoneMapSensor(vector, sensorRadius);
}

const PheromoneMapSensor SensorDriver::getSensorFromAngle(float angle)
{
	Vector2 vector = Vector2::FromAngle(angle);
	vector *= SENSOR_DISTANCE;
	return getSensorFromVector(vector);
}

const Vector2 SensorDriver::getSensorVectorFromAngle(float angle)
{
	Vector2 vector = Vector2::FromAngle(angle);
	vector *= SENSOR_DISTANCE;
	return vector;
}
