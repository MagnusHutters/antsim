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


inline PheromoneMap::coordsFromIndexCoords PheromoneMap::coordsFromIndex(int index) {
	int x = (index % MAP_WIDTH);
	int y = (index / MAP_WIDTH);
	return {x,y};
}

void PheromoneMap::setActive(int positive, int id, int index) {
	if (!pheromonesIsActive[positive][id][index]) {
		pheromonesIsActive[positive][id][index] = true;
		//PheromoneMapIndex(positive, id, index);
		//activeCellActive.push_back(&pheromonesIsActive[positive][id][index]);
		//activeCellValues.push_back(&pheromones[positive][id][index]);
		//activeCellCoords.push_back(coordsFromIndex(index,id,positive));
		auto var = coordsFromIndex(index);
		ActivePheromoneInfo newInfo{
			var.x,
			var.y,
			id,
			positive,
			&pheromones[positive][id][index],
			&pheromonesIsActive[positive][id][index],
			currentClockTick
		};
		activeCellInfo.push_back(newInfo);
		toUpdateCells.push_back(newInfo);

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

unsigned long long PheromoneMap::getCacheKey(int x, int y, int id, int positive, int radius)
{
	unsigned long long key = (x);
	key = (key * MAP_HEIGHT) + (y);
	key = (key * NUMBER_OF_PHEROMONE_PAIRS) + id;
	key = (key * 2) + positive;
	key = (key * 128) + radius;
	return key;
}

Vector2 PheromoneMap::sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id) {
	ensureReadable();
	int sensorSize = (int)sensor.radius;
	int x = (int)sensor.x;
	int y = (int)sensor.y;
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
	int sensorSize = static_cast<int>(sensor.radius);
	int x = static_cast<int>(sensor.x);
	int y = static_cast<int>(sensor.y);

	/*unsigned long long key = getCacheKey(x, y, id, positive, sensorSize);
	auto cache = getStrenghtCache.find(key);
	if (cache != getStrenghtCache.end()) {
		return cache->second;
	}*/


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
	//getStrenghtCache[key] = a;


	return a;

}

float PheromoneMap::sensePheromonesAverage(const PheromoneMapSensor& sensor, int id, bool positive)
{
	ensureReadable();
	int sensorSize = static_cast<int>(sensor.radius);
	int x = static_cast<int>(sensor.x);
	int y = static_cast<int>(sensor.y);
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

Vector2 PheromoneMap::sensePheromonesStrenghtDirection(const PheromoneMapSensor& sensor, Vector2 origin, int id, bool positive, float obstructed) {
	ensureReadable();
	int sensorSize = static_cast<int>(sensor.radius);
	int x = static_cast<int>(sensor.x);
	int y = static_cast<int>(sensor.y);

	unsigned long long key = getCacheKey(x, y, id, positive, sensorSize);

	/*auto cache = getDirectionCache.find(key);
	if (cache != getDirectionCache.end()) {
		return cache->second;
	}*/

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
			(pheromones[positive][id][iterIndex]+(pheromonesIsObstructed[iterIndex]*obstructed));
		//b += pheromones[1][id][iterIndex];
		//i++;
		distributionIterator++;
		vectorIterator++;

	}
	//value.Normalize();

	//getDirectionCache[key] = value;

	return value;

}

void PheromoneMap::getRectOfIndex(std::list<int>& indexList, IntCoords pos, IntCoords size)
{
	for (int x = pos.x; x < pos.x+size.x; ++x)
	{
		for (int y = pos.y; y < pos.y + size.y; ++y)
		{
			indexList.push_back(indexFromCoord(x, y));
		}
	}

}

void PheromoneMap::setObstructedRect(IntCoords pos, IntCoords size)
{
	std::list<int> indexList;
	getRectOfIndex(indexList, pos, size);

	for (int index : indexList)
	{
		pheromonesIsObstructed[index] = 1;
		for (int id = 0; id < NUMBER_OF_PHEROMONE_PAIRS; ++id)
		{
			pheromones[0][id][index] = 0;
			pheromones[1][id][index] = 0;
		}
	}
}

void PheromoneMap::cleatObstructedRect(IntCoords pos, IntCoords size)
{
	std::list<int> indexList;
	getRectOfIndex(indexList, pos, size);

	for (int index : indexList)
	{
		pheromonesIsObstructed[index] = 0;
		
	}

}


void PheromoneMap::doDecayPheromones() {
	ensureWritable();

	

	currentClockTick++;
	//std::list<PheromoneMapIndex> activeCells;

	
	//std::list<float*>::iterator iterValue = activeCellValues.begin();
	//std::list<bool*>::iterator iterActive = activeCellActive.begin();
	//std::list<IntCoords>::iterator iterCoords = activeCellCoords.begin();
	//
	//std::list<float*>::iterator iterValueEnd = activeCellValues.end();

	//std::list< PheromoneMapIndex >::iterator iter = activeCells.begin();
	//std::list< PheromoneMapIndex >::iterator end = activeCells.end();
	
	
	//while (iterInfo != activeCellInfo.end()){
	for (int i = 0; i < 10000; ++i){

	
		//PheromoneMapIndex& p = *iter;
		if(iterInfo== activeCellInfo.end())
		{
			iterInfo = activeCellInfo.begin();
			getDirectionCache.clear();
			getStrenghtCache.clear();
			break;
		}

		//**iterValue *= PHEROMONE_DECAY;
		

		unsigned long ticks_since_update = currentClockTick - iterInfo->lastDecayTick;

		*(iterInfo->value) *= powf(PHEROMONE_DECAY, static_cast<float>(ticks_since_update));

		iterInfo->lastDecayTick = currentClockTick;

		//if (**iterValue < PHEROMONE_FLOOR) {
		if (*(iterInfo->value) < PHEROMONE_FLOOR) {
			*(iterInfo->value)=0;
			*(iterInfo->active)=false;


			clearedCells.push_back(*iterInfo);
			iterInfo = activeCellInfo.erase(iterInfo);
			
		}
		else {
			toUpdateCells.push_back(*iterInfo);
			//iterActive++;
			//iterValue++;
			//iterCoords++;
			iterInfo++;
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

void PheromoneMap::setPheromone(int x, int y, int id, bool positive, float strenght) {
	ensureWritable();
	int index = indexFromCoord(x, y);


	if (!insideBounds(index)) return;

	pheromones[positive][id][index] = strenght;

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
