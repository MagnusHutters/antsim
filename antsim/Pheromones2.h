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

class PheromoneMap {
public:


	PheromoneMap(int sizeX, int sizeY);

	void setReadOnly() {
		readOnly = true;
		writeOnly = false;
	}
	void setWriteOnly(){
		readOnly = false;
		writeOnly = true;
	}

	//READ

	inline float getPheromone(int x, int y, int id, bool positive){
		ensureReadable();
		int index = indexFromCoord(x, y);
		return pheromones[positive][id][index];
	}

	Vector2 sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id);
	float sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id, bool positive);

	float sensePheromonesAverage(const PheromoneMapSensor& sensor, int id, bool positive);

	Vector2 sensePheromonesStrenghtDirection(const PheromoneMapSensor& sensor, Vector2 origin, int id, bool positive);


	//WRITE
	void doDecayPheromones();
	inline void addPheromone(Vector2 pos, int id, bool positive, float strenght) {
		addPheromone(static_cast<int>(pos.x + 0.5), static_cast<int>(pos.y + 0.5), id, positive, 1);
	}
	void addPheromone(int x, int y, int id, bool positive, float strenght);

private:
	bool readOnly = false; 
	bool writeOnly = false;

	void ensureReadable();
	void ensureWritable();

	//std::vector<std::vector<BasePheromoneMap*>> innerPheromones;


	inline bool insideBounds(int index);
	inline int bound(int index);

	inline int indexFromCoord(int x, int y);
	inline void setActive(int positive, int id, int index);

	void createSensorCirle(int radius);

	void ensureSensorCircleExists(int sensorSize);


	std::array<std::array<std::array<float, MAP_CELLS>, NUMBER_OF_PHEROMONE_PAIRS>, 2> pheromones;
	std::array<std::array<std::array<bool, MAP_CELLS>, NUMBER_OF_PHEROMONE_PAIRS>, 2> pheromonesIsActive;
	//std::list<PheromoneMapIndex> activeCells2;
	//std::list< std::array<float, MAP_CELLS>::iterator> activeCells;
	std::list<float*> activeCellValues;
	std::list<bool*> activeCellActive;

	std::unordered_map<int, std::list<int>> sensorCirles;
	std::unordered_map<int, std::vector<float>> sensorCirlesDistribution;
	std::unordered_map<int, std::vector<Vector2>> sensorCirlesVector;
};

