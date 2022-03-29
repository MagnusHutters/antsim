#include "SensorDriver.h"

SensorDriver::SensorDriver(BodyDriver* body, PheromoneMap* pheromoneMap) : body(body), pheromoneMap(pheromoneMap)
{
	resetSensor();

}

void SensorDriver::doSense()
{


	primaryLeftPheromone = pheromoneMap->sensePheromonesStrenght(getSensorFromVector(primaryLeftVector), pheromoneId, 1);
	primaryRightPheromone = pheromoneMap->sensePheromonesStrenght(getSensorFromVector(primaryRightVector), pheromoneId, 1);

}

void SensorDriver::resetSensor()
{
	setPrimarySensorAngle(45);

	enabledPrimarySensor = false;
}

inline void SensorDriver::setPrimarySensorAngle(float angle)
{
	primaryCenterVector = getSensorVectorFromAngle(0);
	primaryLeftVector = getSensorVectorFromAngle(angle);
	primaryRightVector = getSensorVectorFromAngle(-angle);
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
