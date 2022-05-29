#include "PheromoneDriver.h"


PheromoneDriver::PheromoneJob PheromoneDriver::getTrail(PheromoneId pheromone)
{
	auto it = trails.begin();
	while (it != trails.end()) {
		if (it->pheromone == pheromone) {
			return *it;
		}else
		{
			++it;
		}
	}
	return {pheromone, 0, 0, {0,0}};
}

void PheromoneDriver::setTrail(PheromoneId pheromone, float strenght, float strenghtDecay, Vector2 layOffset)
{
	removeTrails();
	addTrail(pheromone, strenght, strenghtDecay, layOffset);
}

void PheromoneDriver::addTrail(PheromoneId pheromone, float strenght, float strenghtDecay, Vector2 layOffset)
{
	removeTrail(pheromone);
	trails.emplace_back(pheromone, strenght, strenghtDecay,layOffset);
}

void PheromoneDriver::removeTrail(PheromoneId pheromone)
{
	auto it= trails.begin();
	while (it != trails.end()) {
		if (it->pheromone==pheromone) {
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

void PheromoneDriver::emit(PheromoneId pheromone)
{
	emits.emplace_back(pheromone);
}
void PheromoneDriver::emit(PheromoneId pheromone, float strenght,Vector2 offset)
{
	emits.emplace_back(pheromone, strenght,0,offset);
}

void PheromoneDriver::update()
{

	auto trail = trails.begin();
	while (trail != trails.end()) {

		trail->strenght *= trail->decay;
		if (trail->strenght < PHEROMONE_FLOOR)
		{
			trail = trails.erase(trail);
		}else
		{
			
			m.pheromoneMap->addPheromone(bodyDriver->body.pos+bodyDriver->getGlobalVector(trail->offset), trail->pheromone.id, trail->pheromone.positive, trail->strenght);
			trail++;
		}

		
		
	}
	
	for (auto emit : emits)
	{
		m.pheromoneMap->addPheromone(bodyDriver->body.pos + bodyDriver->getGlobalVector(emit.offset), emit.pheromone.id, emit.pheromone.positive, emit.strenght);

	}
	emits.clear();
	//pheromoneMap->addPheromone(trailPheromone);
}
