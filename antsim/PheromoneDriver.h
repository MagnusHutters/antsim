#pragma once

#include <list>

#include "Config.h"
#include "Pheromones2.h"
#include "Body.h"

class PheromoneDriver
{
public:

	struct PheromoneJob {
		int id;
		bool positive;
		float strenght;
		PheromoneJob(const int id, const bool positive) : id(id), positive(positive), strenght( 1) {}
		PheromoneJob(const int id, const bool positive, float strenght) : id(id), positive(positive), strenght(strenght) {}
	};

	PheromoneDriver(PheromoneMap* pheromoneMap, BodyDriver* bodyDriver) :
		pheromoneMap(pheromoneMap),
		bodyDriver(bodyDriver)
	{
		
	}
	void setTrail(int id, bool positive=true);
	void addTrail(int id, bool positive=true);
	void removeTrail(int id, bool positive=true);
	void removeTrails();

	//inline void setPheromoneTrail(int pheromoeId);
	// void setExplore() { setPheromoneTrail(EXPLORED_PHEROMONE_ID); }
	//inline void setPath() { setPheromoneTrail(PATH_PHEROMONE_ID); }
	void emit(int id, bool positive);
	void emit(int id, bool positive, float strenght);
	
	inline void success() { emit(SUCCESS_PHEROMONE_ID, true); }
	inline void failure() { emit(SUCCESS_PHEROMONE_ID, false); }

	void update();



private:
	PheromoneMap* pheromoneMap;
	BodyDriver* bodyDriver;


	int trailPheromone=-1;
	std::list<PheromoneJob> emits;
	std::list<PheromoneJob> trails;




};

