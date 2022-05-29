#pragma once



#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"



#include "ActionDriver.h"
#include "PheromoneId.h"

class ActionExplore : public Action
{
public:

	ActionExplore(int id) : exploredPheromone({id,true}), lookFor(false), lookForPheromone({-1, true})
	{

	}

	ActionExplore(PheromoneId pheromoneTrail, bool doLookFor, PheromoneId pheromone) : exploredPheromone(pheromoneTrail), lookForPheromone(pheromone), lookFor(doLookFor)
	{

	}

	ActionExplore(PheromoneId pheromoneTrail) : exploredPheromone(pheromoneTrail), lookFor(false)
	{

	}

	float getStrenght() override;

	void init();

	void calcAction();

private:
	int countdown = 0;
	PheromoneId exploredPheromone;
	PheromoneId lookForPheromone;
	bool lookFor;
	float strenght = 0;
	Vector2 direction;

};
