
#include "AntContainer.h"
#include <stdlib.h>

AntContainer::AntContainer(int numAnts, int sizeX, int sizeY, MapContainer m) :
	sizeX(sizeX), sizeY(sizeY), antIdCount(0), antCount(0), m(m)
{
	srand(RANDOM_SEED);
	setNumAnts(numAnts);

}


void AntContainer::processAnts()
{
	for (auto ant : ants)
	{
		ant->process();
	}
}

void AntContainer::updateAnts()
{
	for (auto ant : ants)
	{
		ant->update();
	}
}





void AntContainer::setNumAnts(int numAnts)
{
	if (numAnts > antCount) {
		createAnts(numAnts - antCount);
	}

}

void AntContainer::createAnts(int numAnts) {

	ants.reserve(antCount + numAnts);


	for (int i = 0; i < numAnts; i++)
	{
		createAnt();
	}

}

void AntContainer::deleteAnts(int numAnts)
{
	for (int i = 0; i < numAnts; i++)
	{
		deleteAnt();
	}
}

void AntContainer::createAnt()
{
	antIdCount = ants.size();

	
	int x = rand() % sizeX;
	int y = rand() % sizeY;
	float rot = static_cast<float>(rand() % 9999);
	//x = 200;
	//y = 100;


	ants.push_back(new Ant(antIdCount, x, y, rot, m));
	antIdCount++;
	
}

void AntContainer::deleteAnt()
{
	antIdCount = ants.size();

	ants.pop_back();
	antIdCount--;
}
