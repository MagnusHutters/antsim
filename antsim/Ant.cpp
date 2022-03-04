#include "Ant.h"
#include <stdlib.h>


Ant::Ant(int id, int x, int y, float rot, PheromoneMap* pheromoneMap)
{
	this->pheromoneMap = pheromoneMap;
	


	bodyDriver = new BodyDriver(x,y,rot);
	//body = Body(x, y, rot);
	
}

void Ant::process()
{
	
	bodyDriver->process();

}

void Ant::update()
{

	pheromoneMap->addPheromone((int)bodyDriver->body.pos.x, (int)bodyDriver->body.pos.y, EXPLORED, true, 1.0);

	bodyDriver->update();


}

Body Ant::getBody()
{
	return bodyDriver->body;
}
