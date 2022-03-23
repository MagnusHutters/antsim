#include "SensorDriver.h"

SensorDriver::SensorDriver(BodyDriver* body, PheromoneMap* pheromoneMap) : body(body), pheromoneMap(pheromoneMap)
{
	
}

void SensorDriver::doSense()
{


	frontLeftPheromone = pheromoneMap->sensePheromonesStrenght(getSensorFromVector(frontLeft), pheromoneId);
	frontRightPheromone = pheromoneMap->sensePheromonesStrenght(getSensorFromVector(frontRight), pheromoneId);

}

inline const PheromoneMapSensor& SensorDriver::getSensorFromVector(Vector2 vector)
{
	

	vector.Rotate(body->body.rot);

	vector += body->body.pos;

	return PheromoneMapSensor(vector, sensorRadius);
}
