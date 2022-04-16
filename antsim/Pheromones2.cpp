#include "Pheromones2.h"


PheromoneMap::PheromoneMap(int sizeX, int sizeY) {

}

inline bool PheromoneMap::insideBounds(int index) {
	return (index < MAP_CELLS&& index >= 0);
}

inline int PheromoneMap::bound(int index) {
	if (index < 0)  index += MAP_CELLS;
	if (index >= MAP_CELLS)  index -= MAP_CELLS;
	return index;
}

inline int PheromoneMap::indexFromCoord(int x, int y) {
	return (y * MAP_WIDTH) + x;
}

void PheromoneMap::setActive(int positive, int id, int index) {
	if (!pheromonesIsActive[positive][id][index]) {
		pheromonesIsActive[positive][id][index] = true;
		//PheromoneMapIndex(positive, id, index);
		activeCellActive.push_back(&pheromonesIsActive[positive][id][index]);
		activeCellValues.push_back(&pheromones[positive][id][index]);
	}

}


void PheromoneMap::createSensorCirle(int radius) {
	int radius2 = radius * radius;
	std::list<int> sensorCirle;
	std::vector<Vector2> sensorCirleVector;
	std::vector<float> sensorCirleDistribution;

	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			if (((x * x) + (y * y)) < radius2) {
				sensorCirle.push_back(indexFromCoord(x, y));
				sensorCirleVector.push_back(Vector2(x, y));
				sensorCirleDistribution.push_back(Vector2(x, y).GetNormalFunction(radius * 0.5));
			}
		}
	}
	sensorCirles[radius] = sensorCirle;
	sensorCirlesVector[radius] = sensorCirleVector;
	sensorCirlesDistribution[radius] = sensorCirleDistribution;
}

void PheromoneMap::ensureSensorCircleExists(int sensorSize) {
	auto search = sensorCirles.find(sensorSize);
	if (search == sensorCirles.end()) {
		createSensorCirle(sensorSize);
	}
}

Vector2 PheromoneMap::sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id) {
	ensureReadable();
	int sensorSize = sensor.radius;
	int x = sensor.x;
	int y = sensor.y;
	int index = indexFromCoord(x, y);
	int iterIndex = 0;

	ensureSensorCircleExists(sensorSize);

	//std::list<int> &sensorCirle = sensorCirles[sensorSize];
	float a = 0;
	float b = 0;

	for (auto const& i : sensorCirles[sensorSize]) {
		iterIndex = index + i;
		iterIndex = bound(iterIndex);

		a += pheromones[0][id][iterIndex];
		b += pheromones[1][id][iterIndex];

	}
	return Vector2(a, b);


}

float PheromoneMap::sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id, bool positive) {
	ensureReadable();
	int sensorSize = sensor.radius;
	int x = sensor.x;
	int y = sensor.y;
	int index = indexFromCoord(x, y);
	int iterIndex = 0;

	ensureSensorCircleExists(sensorSize);


	//std::list<int> &sensorCirle = sensorCirles[sensorSize];
	float a = 0;
	//float b = 0;

	for (auto const& i : sensorCirles[sensorSize]) {
		iterIndex = index + i;
		iterIndex = bound(iterIndex);

		a += pheromones[positive][id][iterIndex];
		//b += pheromones[1][id][iterIndex];

	}
	return a;

}

float PheromoneMap::sensePheromonesAverage(const PheromoneMapSensor& sensor, int id, bool positive)
{
	ensureReadable();
	int sensorSize = sensor.radius;
	int x = sensor.x;
	int y = sensor.y;
	int index = indexFromCoord(x, y);
	int iterIndex = 0;

	ensureSensorCircleExists(sensorSize);


	//std::list<int> &sensorCirle = sensorCirles[sensorSize];
	float a = 0;
	//float b = 0;

	for (auto const& i : sensorCirles[sensorSize]) {
		iterIndex = index + i;
		iterIndex = bound(iterIndex);

		a += pheromones[positive][id][iterIndex];
		//b += pheromones[1][id][iterIndex];

	}
	return a / (float)sensorCirles[sensorSize].size();
}

Vector2 PheromoneMap::sensePheromonesStrenghtDirection(const PheromoneMapSensor& sensor, Vector2 origin, int id, bool positive) {
	ensureReadable();
	int sensorSize = sensor.radius;
	int x = sensor.x;
	int y = sensor.y;
	int index = indexFromCoord(x, y);
	int iterIndex = 0;


	ensureSensorCircleExists(sensorSize);


	//std::list<int> &sensorCirle = sensorCirles[sensorSize];
	//float a = 0;
	//float b = 0;

	Vector2 value = Vector2(0, 0);
	float totalStrenght = 0;


	int i = 0;


	//std::list<int> sensorCirles;
	std::vector<float>::iterator distributionIterator = sensorCirlesDistribution[sensorSize].begin();
	std::vector<Vector2>::iterator vectorIterator = sensorCirlesVector[sensorSize].begin();


	for (auto const& circleIndex : sensorCirles[sensorSize]) {
		iterIndex = index + circleIndex;
		iterIndex = bound(iterIndex);

		//float strenght= pheromones[positive][id][iterIndex];
		//totalStrenght += 
		value +=
			*vectorIterator *
			*distributionIterator *
			pheromones[positive][id][iterIndex];
		//b += pheromones[1][id][iterIndex];
		//i++;
		distributionIterator++;
		vectorIterator++;

	}
	//value.Normalize();
	return value;

}

void PheromoneMap::doDecayPheromones() {
	ensureWritable();
	//std::list<PheromoneMapIndex> activeCells;

	std::list<float*>::iterator iterValue = activeCellValues.begin();
	std::list<bool*>::iterator iterActive = activeCellActive.begin();

	std::list<float*>::iterator iterValueEnd = activeCellValues.end();

	//std::list< PheromoneMapIndex >::iterator iter = activeCells.begin();
	//std::list< PheromoneMapIndex >::iterator end = activeCells.end();

	while (iterValue != iterValueEnd)
	{
		//PheromoneMapIndex& p = *iter;
		
		**iterValue *= PHEROMONE_DECAY;
		if (**iterValue < PHEROMONE_FLOOR) {
			**iterValue = 0;
			**iterActive = false;
			//doDelete = true;

			iterActive = activeCellActive.erase(iterActive);
			iterValue = activeCellValues.erase(iterValue);
		}
		else {

			iterActive++;
			iterValue++;
		}
		/*
		pheromones[p.positive][p.id][p.index] *= PHEROMONE_DECAY;
		if (pheromones[p.positive][p.id][p.index] < PHEROMONE_FLOOR) {
		pheromones[p.positive][p.id][p.index] = 0;
		pheromonesIsActive[p.positive][p.id][p.index] = false;
		doDelete = true;

		}*/

	}

}

void PheromoneMap::addPheromone(int x, int y, int id, bool positive, float strenght) {
	ensureWritable();
	int index = indexFromCoord(x, y);


	if (!insideBounds(index)) return;

	pheromones[positive][id][index] += strenght;

	setActive(positive, id, index);
}



inline void PheromoneMap::ensureReadable() {
	if (!readOnly) {
		throw std::runtime_error("Cannot read pheromones");
	}
}

inline void PheromoneMap::ensureWritable() {
	if (!writeOnly) {
		throw std::runtime_error("cannot write pheromones");
	}
}
