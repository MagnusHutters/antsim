#pragma once

#include <SFML/Graphics.hpp>


class Core;

#include "Core.h"

#include <vector>

#define FRAME_TIME 60
#define PHEROMONE_UPDATE_RATE 8

class Graphic
{

public:

	Graphic();
	Graphic(Core* core, int resolutionMultiplier);

	void update();

private:

	Core* core;
	sf::RenderWindow* window;

	int pheromoneCounter = 0;
	float elapsedTime;
	int pheromoneSize;
	int resolutionMultiplier;
	int worldSizeX, worldSizeY, windowSizeX, windowSizeY;
	sf::Texture pheromoneTexture;
	sf::Image pheromoneImage;
	sf::Sprite pheromoneSprite;

	sf::Clock clock;


	std::vector< sf::CircleShape > antShapes;



	void drawPheromones();
	void drawAnts();
	void ajustAntSize(int newSize);

};

