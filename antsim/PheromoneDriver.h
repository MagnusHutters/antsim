#pragma once

#include <list>

#include "Config.h"
#include "Pheromones2.h"
#include "Body.h"
#include "Conditions.h"
#include "PheromoneId.h"

class PheromoneDriver
{
public:

	struct PheromoneJob {
		PheromoneId pheromone;
		float strenght;
		float decay;
		PheromoneJob(PheromoneId pheromone) : pheromone(pheromone), strenght(1), decay(1) {}
		PheromoneJob(PheromoneId pheromone, float strenght, float decay=1) : pheromone(pheromone), strenght(strenght), decay(decay) {}
	};

	PheromoneDriver(PheromoneMap* pheromoneMap, BodyDriver* bodyDriver) :
		pheromoneMap(pheromoneMap),
		bodyDriver(bodyDriver)
	{
		
	}
	PheromoneJob getTrail(PheromoneId pheromone);
	void setTrail(PheromoneId pheromone, float strenght = 1, float strenghtDecay = 1);
	void addTrail(PheromoneId pheromone, float strenght = 1, float strenghtDecay = 1);
	void removeTrail(PheromoneId pheromone);
	void removeTrails();

	//inline void setPheromoneTrail(int pheromoeId);
	// void setExplore() { setPheromoneTrail(EXPLORED_PHEROMONE_ID); }
	//inline void setPath() { setPheromoneTrail(PATH_PHEROMONE_ID); }
	void emit(PheromoneId pheromone);
	void emit(PheromoneId pheromone, float strenght);
	
	inline void success() { emit({SUCCESS_PHEROMONE_ID, true}); }
	inline void failure() { emit({SUCCESS_PHEROMONE_ID, false}); }

	void update();



private:
	PheromoneMap* pheromoneMap;
	BodyDriver* bodyDriver;


	int trailPheromone=-1;
	std::list<PheromoneJob> emits;
	std::list<PheromoneJob> trails;




};

