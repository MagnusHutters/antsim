#include "SensorDriver.h"

SensorDriver::SensorDriver(BodyDriver* body, PheromoneMap* pheromoneMap, EntityMap<Job>* jobMap) : body(body), pheromoneMap(pheromoneMap), jobMap(jobMap)
{
	resetSensor();

}

void SensorDriver::doSense()
{
	if (enabledPrimarySensor) {
		primaryLeftPheromoneResult = pheromoneMap->sensePheromonesStrenght(getSensorFromVector(primaryLeftVector), primaryPheromoneId, 1);
		primaryRightPheromoneResult = pheromoneMap->sensePheromonesStrenght(getSensorFromVector(primaryRightVector), primaryPheromoneId, 1);
	}
	if (enabledDirectionSensor) {
		directionSensorResult = pheromoneMap->sensePheromonesStrenghtDirection(getSensorAtBody(SENSOR_DIRECTION_RADIUS), body->body.pos, directionPheromoneId, directionSensorSignum);
	}


}

void SensorDriver::resetSensor()
{
	setPrimarySensorAngle(45);

	primaryPheromoneId = -1;
	directionPheromoneId = -1;

	enabledDirectionSensor = false;
	enabledPrimarySensor = false;
}

inline void SensorDriver::setPrimarySensorAngle(float angle)
{
	primaryCenterVector = getSensorVectorFromAngle(0);
	primaryLeftVector = getSensorVectorFromAngle(angle);
	primaryRightVector = getSensorVectorFromAngle(-angle);
}

inline const PheromoneMapSensor& SensorDriver::getSensorAtBody(int radius)
{
	return PheromoneMapSensor(body->body.pos, radius);
}

inline const PheromoneMapSensor& SensorDriver::getSensorFromVector(Vector2 vector)
{
	

	vector.Rotate(body->body.rot);

	vector += body->body.pos;

	return PheromoneMapSensor(vector, sensorRadius);
}

const PheromoneMapSensor& SensorDriver::getSensorFromAngle(float angle)
{
	Vector2 vector = Vector2::FromAngle(angle);
	vector *= SENSOR_DISTANCE;
	return getSensorFromVector(vector);
}

const Vector2& SensorDriver::getSensorVectorFromAngle(float angle)
{
	Vector2 vector = Vector2::FromAngle(angle);
	vector *= SENSOR_DISTANCE;
	return vector;
}
