#include "Pheromones.h"
#include <iostream>


void BasePheromoneMap::_addPheromone(PheromoneMapParams* p) {
	int halfSize = size / 2;
	if (p->x < xpos + halfSize) {
		if (p->y < ypos + halfSize) {
			children[0]->_addPheromone(p);
		}
		else {
			children[1]->_addPheromone(p);
		}
	}
	else {
		if (p->y < ypos + halfSize) {
			children[2]->_addPheromone(p);
		}
		else {
			children[3]->_addPheromone(p);
		}
	}


	contains[p->positive] |= p->bitMap;
}



bool BasePheromoneMap::_decayPheromone(PheromoneMapParams* p) {
	bool positive = p->positive;
	if (contains[positive] & p->bitMap) {
		bool doUpdate = false;
		doUpdate |= children[0]->_decayPheromone(p);
		doUpdate |= children[1]->_decayPheromone(p);
		doUpdate |= children[2]->_decayPheromone(p);
		doUpdate |= children[3]->_decayPheromone(p);
		if (doUpdate) {

			uint32_t bitMask = ~((children[0]->contains[positive] | children[1]->contains[positive] | children[2]->contains[positive] | children[3]->contains[positive]) & (~p->bitMap));
			contains[positive] &= bitMask;
			return true;
		}
	}
	return false;
}

//Constructor


InnerPheromoneMap::InnerPheromoneMap() {

}

InnerPheromoneMap::InnerPheromoneMap(int size, int xpos, int ypos, BasePheromoneMap* outer) {
	this->size = size;
	this->xpos = xpos;
	this->ypos = ypos;
	this->outer = outer;

	pheromones.resize(NUMBER_OF_PHEROMONE_PAIRS, std::vector<float>(2));


	outer->registerInner(this, xpos, ypos);
}

void InnerPheromoneMap::_addPheromone(PheromoneMapParams* p) {

	contains[p->positive] |= p->bitMap;
	pheromones[p->id][p->positive] += p->strenght;
}

bool InnerPheromoneMap::_decayPheromone(PheromoneMapParams* p) {
	bool positive = p->positive;
	if (contains[positive] & p->bitMap) {
		pheromones[p->id][positive] *= PHEROMONE_DECAY;
		if (pheromones[p->id][positive] < PHEROMONE_FLOOR) {
			pheromones[p->id][positive] = 0;
			contains[positive] &= ~p->bitMap;
			return true;
		}
	}
	return false;
}


MiddlePheromoneMap::MiddlePheromoneMap() {
}

MiddlePheromoneMap::MiddlePheromoneMap(int size, int xpos, int ypos, BasePheromoneMap* outer) {
	this->size = size;
	this->xpos = xpos;
	this->ypos = ypos;
	this->outer = outer;


	if (size > 2) {
		int newSize = size / 2;
		children[0] = new MiddlePheromoneMap(size / 2, xpos, ypos,						outer);
		children[1] = new MiddlePheromoneMap(size / 2, xpos , ypos + newSize,			outer);
		children[2] = new MiddlePheromoneMap(size / 2, xpos+ newSize, ypos,				outer);
		children[3] = new MiddlePheromoneMap(size / 2, xpos + newSize, ypos + newSize,	outer);
	}
	else {
		int newSize = size / 2;
		children[0] = new InnerPheromoneMap(size / 2, xpos, ypos, outer);
		children[1] = new InnerPheromoneMap(size / 2, xpos, ypos + newSize, outer);
		children[2] = new InnerPheromoneMap(size / 2, xpos + newSize, ypos, outer);
		children[3] = new InnerPheromoneMap(size / 2, xpos + newSize, ypos + newSize, outer);
	}
}


void MiddlePheromoneMap::_addPheromone(PheromoneMapParams* p) {

	int halfSize = size / 2;
	if (p->x < xpos + halfSize) {
		if (p->y < ypos + halfSize) {
			children[0]->_addPheromone(p);
		}
		else {
			children[1]->_addPheromone(p);
		}
	}
	else {
		if (p->y < ypos + halfSize) {
			children[2]->_addPheromone(p);
		}
		else {
			children[3]->_addPheromone(p);
		}
	}


	contains[p->positive] |= p->bitMap;
}





PheromoneMap::PheromoneMap(int sizeX, int sizeY) {
	size = std::max(sizeX, sizeY);
	xpos = 0;
	ypos = 0;
	outer = this;

	innerPheromones.resize(size, std::vector<BasePheromoneMap*>(size));

	int newSize = size / 2;
	children[0] = new MiddlePheromoneMap(size / 2, xpos, ypos, outer);
	children[1] = new MiddlePheromoneMap(size / 2, xpos, ypos + newSize, outer);
	children[2] = new MiddlePheromoneMap(size / 2, xpos + newSize, ypos, outer);
	children[3] = new MiddlePheromoneMap(size / 2, xpos + newSize, ypos + newSize, outer);

}

void PheromoneMap::registerInner(BasePheromoneMap* inner, int x, int y) {
	innerPheromones[x][y] = inner;
}

void PheromoneMap::doDecayPheromones() {
	for (int i = 0; i < NUMBER_OF_PHEROMONE_PAIRS; i++)
	{
		uint32_t bitMap = (uint32_t)1 << i;

		PheromoneMapParams* params = new PheromoneMapParams(bitMap, i, false);

		_decayPheromone(params);
		params->positive = true;
		_decayPheromone(params);
		delete(params);
	}
}

void PheromoneMap::addPheromone(int x, int y, int id, bool positive, float strenght) {
	uint32_t bitMap = (uint32_t)1 << id;
	PheromoneMapParams* params = new PheromoneMapParams(x, y, bitMap, id, positive, strenght);

	_addPheromone(params);

	delete(params);
}

//bool unsetContains(int x, int y, int id, bool positive = true) {
//	uint32_t bitMap = (uint32_t)1 << id;
//	//return unsetContainsBit(x, y, bitMap, positive);
//}
