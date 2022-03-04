#pragma once

#include <SFML/Graphics.hpp>


class Core;

#include "Core.h"

#include <vector>

#define FRAME_TIME 30


class Graphic
{

public:

	Graphic();
	Graphic(Core* core, int resolutionMultiplier);

	void update();

private:

	Core* core;
	sf::RenderWindow* window;


	float elapsedTime;
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

