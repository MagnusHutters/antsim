#include "Graphic.h"
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Body.h"

Graphic::Graphic(Core* core, int resolutionMultiplier)
{
	this->core = core;
	worldSizeX = core->world->getSizeX();
	worldSizeY = core->world->getSizeY();
	this->resolutionMultiplier = resolutionMultiplier;

	windowSizeX = worldSizeX * resolutionMultiplier;
	windowSizeY = worldSizeY * resolutionMultiplier;



	window = new sf::RenderWindow(sf::VideoMode(windowSizeX, windowSizeY), "window");

	int size = core->world->pheromoneMap->size;
	pheromoneTexture.create(size, size);
	pheromoneImage.create(size, size);
	pheromoneSprite.setTexture(pheromoneTexture);
	pheromoneSprite.setScale(resolutionMultiplier, resolutionMultiplier);


	//pheromoneTexture.update();

	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	
}

void Graphic::update()
{


	
	sf::Time elapsed = clock.restart();
	elapsedTime += elapsed.asMilliseconds();
	if (elapsedTime < FRAME_TIME) {
		return;
	}
	else {
		elapsedTime -= FRAME_TIME;
	}


	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
	}

	window->clear(sf::Color::White);

	drawPheromones();
	drawAnts();
	//window->draw(shape);
	window->display();
}


void Graphic::drawPheromones()
{
	int size = core->world->pheromoneMap->size;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			float pheromone1 = core->world->pheromoneMap->innerPheromones[x][y]->pheromones[EXPLORED][true];
			if (pheromone1 > 0) {
				1 + 1;
			}
			sf::Uint8 pheromone1Color = 255 - (sqrt(pheromone1) * 70);
			pheromoneImage.setPixel(x, y, sf::Color(255, pheromone1Color, pheromone1Color));
		}
	}
	pheromoneTexture.update(pheromoneImage);
	window->draw(pheromoneSprite);
}

void Graphic::drawAnts()
{
	std::vector<Body> antBodies = core->world->antContatiner->getAntBodies();

	if (antBodies.size() != antShapes.size()) {
		ajustAntSize( antBodies.size() );
	}

	int i = 0;

	

	for (auto const& body : antBodies) {
		antShapes[i].setPosition(body.pos.x*resolutionMultiplier, body.pos.y*resolutionMultiplier);
		antShapes[i].setRotation(body.rot+90);
		

		window->draw(antShapes[i]);
		i++;
	}
}

void Graphic::ajustAntSize(int newSize)
{
	antShapes.reserve(newSize);

	int ajustment = newSize - antShapes.size();

	if (ajustment > 0) {
		for (int i = 0; i < ajustment; i++)
		{
			antShapes.push_back(sf::CircleShape(5, 3));
			antShapes.back().setFillColor(sf::Color::Black);
			antShapes.back().setOrigin(5,5);

		}
	}
	else {
		for (int i = 0; i > ajustment; i--)
		{
			antShapes.pop_back();
		}
	}
}
