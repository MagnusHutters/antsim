#include "PheromoneDriver.h"



void PheromoneDriver::setTrail(int id, bool positive)
{
	removeTrails();
	addTrail(id, positive);
}

void PheromoneDriver::addTrail(int id, bool positive)
{
	removeTrail(id, positive);
	trails.emplace_back(id, positive);
}

void PheromoneDriver::removeTrail(int id, bool positive)
{
	auto it= trails.begin();
	while (it != trails.end()) {
		if (it->id == id && it->positive == positive) {
			it = trails.erase(it);
		}
		else {
			++it;
		}
	}
	
}

void PheromoneDriver::removeTrails()
{
	trails.clear();
}

void PheromoneDriver::emit(int id, bool positive)
{
	emits.emplace_back(id, positive);
}
void PheromoneDriver::emit(int id, bool positive, float strenght)
{
	emits.emplace_back(id, positive, strenght);
}

void PheromoneDriver::update()
{
	for (auto trail : trails)
	{
		pheromoneMap->addPheromone(bodyDriver->body.pos, trail.id, trail.positive, trail.strenght);

	}
	for (auto emit : emits)
	{
		pheromoneMap->addPheromone(bodyDriver->body.pos, emit.id, emit.positive, emit.strenght);

	}
	emits.clear();
	//pheromoneMap->addPheromone(trailPheromone);
}
