#include "SensorDriver.h"

#include "ObstacleMap.h"

SensorDriver::SensorDriver(BodyDriver* body, MapContainer m) : body(body), m(m)
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



Vector2 SensorDriver::senseDirection(PheromoneId pheromone, float obstructionResponse) { 
	return m.pheromoneMap->sensePheromonesStrenghtDirection(getSensorAtBody(SENSOR_RADIUS_HUGE), body->body.pos, pheromone.id, pheromone.positive, obstructionResponse);
 
}

float SensorDriver::senseStrenght(PheromoneId pheromone)
{
	return m.pheromoneMap->sensePheromonesStrenght(getSensorAtBody(SENSOR_RADIUS_HUGE), pheromone.id, pheromone.positive);
	
}

Vector2 SensorDriver::senseObstructions()
{
	Vector2 pos = body->body.pos;
	auto obstructionPoints = m.bodyCollosionMap->getBodiesInRange(pos, OBSTRUCTION_SENSE_RADIUS*2);


	Vector2 cumulative = {0,0};
	for (auto obstruction_point : obstructionPoints)
	{
		if (body->handle == obstruction_point.handle) continue;
		Vector2 dif = obstruction_point.pos - pos;
		float distDoubleRadier = dif.Length()*(1.0/ (RIGID_BODY_RADIUS*2.0f));
		if(distDoubleRadier==0)
		{
			continue;
		}
		dif /= powf(distDoubleRadier, 3);
		cumulative += dif;

	}
	return cumulative;
}

Vector2 SensorDriver::senseTerrain()
{
	Vector2 pos = body->body.pos;
	return m.obstacleMap->getObstructionVector(pos);
}

inline const PheromoneMapSensor SensorDriver::getSensorAtBody(int radius)
{
	return PheromoneMapSensor(body->body.pos, static_cast<float>(radius));
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
