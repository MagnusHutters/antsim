#pragma once

#include <vector>
#include <cstdint>
#include <cmath> 
#include <algorithm> 
#include "Vector2.h"
#include <mutex>
#include <math.h>
#include <array>

#include <unordered_map>
#include <list>

#include "Config.h"



const struct PheromoneMapIndex {
	int positive;
	int id;
	int index;
	PheromoneMapIndex(int positive, int id, int index) : positive(positive), id(id), index(index) {

	}
};

const struct PheromoneMapSensor {

	float x, y, radius, radius2;
	Vector2 vector;

	PheromoneMapSensor() {
		x = 0; y = 0; radius = 0; radius2 = 0;

	}
	PheromoneMapSensor(float x, float y, float radius) : x(x), y(y), radius(radius), radius2(radius* radius), vector(Vector2(x, y)) {

	}
	PheromoneMapSensor(Vector2 vector, float radius) : x(vector.x), y(vector.y), radius(radius), radius2(radius* radius), vector(vector) {

	}
	PheromoneMapSensor setX(float newX) { return PheromoneMapSensor(newX, y, radius); }
	PheromoneMapSensor setY(float newY) { return PheromoneMapSensor(x, newY, radius); }
	PheromoneMapSensor setPos(float newX, float newY) { return PheromoneMapSensor(newX, newY, radius); }
	PheromoneMapSensor setPos(Vector2 vector) { return PheromoneMapSensor(vector, radius); }
	PheromoneMapSensor setRadius(float radius) { return PheromoneMapSensor(x, y, radius); }


};

struct PheromoneMapParams {
	int x;
	int y;
	uint32_t bitMap;
	int id;
	bool positive;
	float strenght;
	PheromoneMapSensor sensor;
	Vector2 origin;

	PheromoneMapParams(int x, int y, uint32_t bitMap, int id, bool positive, float strenght) :
		x(x),
		y(y),
		bitMap(bitMap),
		id(id),
		positive(positive),
		strenght(strenght),
		sensor(PheromoneMapSensor()),
		origin(Vector2())
	{}


	PheromoneMapParams(uint32_t bitMap, int id, bool positive) :
		x(0),
		y(0),
		bitMap(bitMap),
		id(id),
		positive(positive),
		strenght(0),
		sensor(PheromoneMapSensor()),
		origin(Vector2())
	{}

	PheromoneMapParams(const Vector2& origin, const PheromoneMapSensor& sensor, uint32_t bitMap, int id, bool positive) :
		x(0),
		y(0),
		bitMap(bitMap),
		id(id),
		positive(positive),
		strenght(0),
		sensor(sensor),
		origin(Vector2())
	{}

};



class PheromoneMap {
public:

	//std::vector<std::vector<BasePheromoneMap*>> innerPheromones;



	std::array<std::array<std::array<float, MAP_CELLS>, NUMBER_OF_PHEROMONE_PAIRS>, 2> pheromones{};
	std::array<std::array<std::array<bool, MAP_CELLS>, NUMBER_OF_PHEROMONE_PAIRS>, 2> pheromonesIsActive{};
	std::list<PheromoneMapIndex> activeCells;

	std::unordered_map<int, std::list<int>> sensorCirles;
	std::unordered_map<int, std::vector<float>> sensorCirlesDistribution;
	std::unordered_map<int, std::vector<Vector2>> sensorCirlesVector;

	PheromoneMap(int sizeX, int sizeY){

	}

	inline bool insideBounds(int index) {
		return (index < MAP_CELLS&& index >= 0);
	}
	inline int bound(int index) {
		if (index < 0)  index += MAP_CELLS;
		if (index >= MAP_CELLS)  index -= MAP_CELLS;
		return index;
	}

	inline int indexFromCoord(int x, int y) {
		return (y * MAP_WIDTH) + x;
	}
	inline void setActive(int positive, int id, int index) {
		if (!pheromonesIsActive[positive][id][index]) {
			pheromonesIsActive[positive][id][index] = true;
			activeCells.push_back(PheromoneMapIndex(positive, id, index));
		}

	}

	inline float getPheromone(int x, int y, int id, bool positive) {
		int index = indexFromCoord(x, y);
		return pheromones[positive][ id][index];
	}


	//Vector2& sensePheromonesVector(const Vector2& origin, const PheromoneMapSensor& sensor, int id, bool positive);

	//float sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id, bool positive);


	void createSensorCirle(int radius) {
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

	void ensureSensorCircleExists(int sensorSize) {
		auto search = sensorCirles.find(sensorSize);
		if (search == sensorCirles.end()) {
			createSensorCirle(sensorSize);
		}
	}

	Vector2 sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id) {
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

	float sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id, bool positive) {
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


	Vector2 sensePheromonesStrenghtDirection(const PheromoneMapSensor& sensor, Vector2 origin, int id, bool positive) {
		int sensorSize = sensor.radius;
		int x = sensor.x;
		int y = sensor.y;
		int index = indexFromCoord(x, y);
		int iterIndex = 0;


		ensureSensorCircleExists(sensorSize);


		//std::list<int> &sensorCirle = sensorCirles[sensorSize];
		//float a = 0;
		//float b = 0;

		Vector2 value=Vector2(0,0);
		float strenght = 0;


		int i = 0;
		for (auto const& circleIndex : sensorCirles[sensorSize]) {
			iterIndex = index + circleIndex;
			iterIndex = bound(iterIndex);


			strenght += pheromones[positive][id][iterIndex];
			value +=
				sensorCirlesVector[sensorSize][i] *
				sensorCirlesDistribution[sensorSize][i] *
				pheromones[positive][id][iterIndex];
			//b += pheromones[1][id][iterIndex];
			i++;

		}
		//value.Normalize();
		return value*strenght* strenght;

	}

	

	void doDecayPheromones() {
		//std::list<PheromoneMapIndex> activeCells;

		std::list< PheromoneMapIndex >::iterator iter = activeCells.begin();
		std::list< PheromoneMapIndex >::iterator end = activeCells.end();

		while (iter != end)
		{
			PheromoneMapIndex& p = *iter;

			bool doDelete = false;
			pheromones[p.positive][p.id][p.index] *= PHEROMONE_DECAY;
			if (pheromones[p.positive][p.id][p.index] < PHEROMONE_FLOOR) {
				pheromones[p.positive][p.id][p.index] = 0;
				pheromonesIsActive[p.positive][p.id][p.index] = false;
				doDelete = true;

			}

			if (doDelete)
			{
				iter = activeCells.erase(iter);
			}
			else
			{
				++iter;
			}
		}

	}
	void addPheromone(int x, int y, int id, bool positive, float strenght) {

		int index = indexFromCoord(x, y);

		
		if (!insideBounds(index)) return;

		pheromones[positive][id][index] += strenght;

		setActive(positive, id, index);
	}

};

