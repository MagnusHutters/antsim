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
#include "PheromoneUtility.h"


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

	unsigned long long getCacheKey(int x, int y, int id, int positive, int radius);

	Vector2 sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id);
	float sensePheromonesStrenght(const PheromoneMapSensor& sensor, int id, bool positive);

	float sensePheromonesAverage(const PheromoneMapSensor& sensor, int id, bool positive);

	Vector2 sensePheromonesStrenghtDirection(const PheromoneMapSensor& sensor, Vector2 origin, int id, bool positive, float Obstructed=0);


	void getRectOfIndex(std::list<int>& indexList, IntCoords pos, IntCoords size);

	void setObstructedRect(IntCoords pos, IntCoords size);

	void cleatObstructedRect(IntCoords pos, IntCoords size);

	//WRITE
	//DECAY=================================================================================
	void doDecayPheromones();
	//DECAY=================================================================================

	inline void addPheromone(Vector2 pos, int id, bool positive, float strenght) {
		addPheromone(static_cast<int>(pos.x), static_cast<int>(pos.y), id, positive, strenght);
	}
	void addPheromone(int x, int y, int id, bool positive, float strenght);

	inline void setPheromone(Vector2 pos, int id, bool positive, float strenght) {
		setPheromone(static_cast<int>(pos.x), static_cast<int>(pos.y), id, positive, strenght);
	}
	void setPheromone(int x, int y, int id, bool positive, float strenght);


	PheromoneActiveReturn getActivePheromones(bool complete)
	{
		if(complete)
		{
			PheromoneActiveReturn toReturn{ &activeCellInfo, {} };
			clearedCells.clear();
			return toReturn;
		}else
		{
			
			PheromoneActiveReturn toReturn{ &toUpdateCells, std::move(clearedCells) };
			clearedCells.clear();
			return toReturn;
		}
	}

	void resetToUpdate()
	{
		toUpdateCells.clear();
	}

	
	std::array<float, MAP_CELLS> getObstrcuted()
	{
		return pheromonesIsObstructed;
	}

private:
	friend class Logger;
	bool readOnly = false; 
	bool writeOnly = false;

	void ensureReadable();
	void ensureWritable();

	//std::vector<std::vector<BasePheromoneMap*>> innerPheromones;


	inline bool insideBounds(int index);
	inline int bound(int index);

	inline int indexFromCoord(int x, int y);


	struct coordsFromIndexCoords { int x; int y; };
	coordsFromIndexCoords coordsFromIndex(int index);
	inline void setActive(int positive, int id, int index);

	void createSensorCirle(int radius);

	void ensureSensorCircleExists(int sensorSize);


	std::array<std::array<std::array<float, MAP_CELLS>, NUMBER_OF_PHEROMONE_PAIRS>, 2> pheromones;
	std::array<float, MAP_CELLS> pheromonesIsObstructed;
	std::array<std::array<std::array<bool, MAP_CELLS>, NUMBER_OF_PHEROMONE_PAIRS>, 2> pheromonesIsActive;
	//std::list<PheromoneMapIndex> activeCells2;
	//std::list< std::array<float, MAP_CELLS>::iterator> activeCells;
	//std::list<float*> activeCellValues;
	//std::list<bool*> activeCellActive;
	//std::list<IntCoords> activeCellCoords;
	std::list<ActivePheromoneInfo> clearedCells;
	std::list<ActivePheromoneInfo> toUpdateCells;

	std::list<ActivePheromoneInfo> activeCellInfo;

	std::list<ActivePheromoneInfo>::iterator iterInfo = activeCellInfo.begin();

	std::unordered_map<unsigned long long, float> getStrenghtCache;
	std::unordered_map<unsigned long long, Vector2> getDirectionCache;

	std::unordered_map<int, std::list<int>> sensorCirles;
	std::unordered_map<int, std::vector<float>> sensorCirlesDistribution;
	std::unordered_map<int, std::vector<Vector2>> sensorCirlesVector;


	unsigned long currentClockTick = 0;


	


};

