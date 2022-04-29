#pragma once



#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"



#include "ActionDriver.h"
#include "PheromoneId.h"

class ActionExplore : public Action
{
public:

	ActionExplore(int id) : exploredPheromone({id,true}), lookFor(false)
	{

	}

	ActionExplore(int id, bool doLookFor, PheromoneId pheromone) : exploredPheromone({ id,true }), lookForPheromone(pheromone), lookFor(doLookFor)
	{

	}

	ActionExplore(PheromoneId PheromoneId) : exploredPheromone(PheromoneId), lookFor(false)
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
