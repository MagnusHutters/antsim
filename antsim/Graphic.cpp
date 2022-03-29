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

	int size = core->world->getPheromoneSize();
	pheromoneSize = size;
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
	pheromoneCounter++;
	if (pheromoneCounter > PHEROMONE_UPDATE_RATE) {
		int size = pheromoneSize;
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				float pheromone1 = core->world->getPheromone(x, y, EXPLORED, 1)*0.05;
				

				float value = sinh(pheromone1) / cosh(pheromone1);

				sf::Uint8 pheromone1Color = 255 - (value*255);
				pheromoneImage.setPixel(x, y, sf::Color(255, pheromone1Color, pheromone1Color));
			}
		}
		pheromoneTexture.update(pheromoneImage);
		pheromoneCounter = 0;
	}

	

	
	window->draw(pheromoneSprite);
}

void Graphic::drawAnts()
{
	
	std::vector<Body> antBodies = core->world->getAntBodies();

	if (antBodies.size() != antShapes.size()) {
		ajustAntSize( antBodies.size() );
	}

	int i = 0;

	

	for (auto const& body : antBodies) {
		antShapes[i].setPosition(body.pos.x*resolutionMultiplier, body.pos.y*resolutionMultiplier);
		antShapes[i].setRotation(body.rot+90);
		

		window->draw(antShapes[i]);

		Vector2 pos1 = core->world->getSensorLocation(0, i)* resolutionMultiplier;
		Vector2 pos2 = core->world->getSensorLocation(1, i)* resolutionMultiplier;
		antSensorLeft[i].setPosition(pos1.x, pos1.y);
		antSensorRight[i].setPosition(pos2.x, pos2.y);

		


		i++;
	}
	window->draw(antSensorLeft[0]);
	window->draw(antSensorRight[0]);

	for (int i = 0; i < antBodies.size(); i++)
	{
		
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

			antSensorLeft.push_back(sf::CircleShape(SENSOR_RADIUS * resolutionMultiplier));
			antSensorLeft.back().setFillColor(sf::Color::Yellow);
			antSensorLeft.back().setOrigin(SENSOR_RADIUS * resolutionMultiplier, SENSOR_RADIUS * resolutionMultiplier);

			antSensorRight.push_back(sf::CircleShape(SENSOR_RADIUS * resolutionMultiplier));
			antSensorRight.back().setFillColor(sf::Color::Cyan);
			antSensorRight.back().setOrigin(SENSOR_RADIUS * resolutionMultiplier, SENSOR_RADIUS * resolutionMultiplier);



		}
	}
	else {
		for (int i = 0; i > ajustment; i--)
		{
			antShapes.pop_back();
			antSensorLeft.pop_back();
			antSensorRight.pop_back();

		}
	}
}
