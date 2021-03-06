#pragma once

#include <SFML/Graphics.hpp>
#include "EntityMap.h"
#include "Job.h"

class GraphicAnt;

#include "GraphicAnt.h"



class Core;

#include "Core.h"

#include <vector>
#include <unordered_map>

#define FRAME_TIME 60
#define PHEROMONE_UPDATE_RATE 10

class Graphic
{

public:

	Graphic();
	Graphic(Core* core, float resolutionMultiplier);

	void update();

private:

	int numAnts = 0;

	std::list<GraphicAnt*> antsGraphics;

	float deltaTime = 30;
	float deltaTimeAvgIndex = 0.1;

	Core* core;
	sf::RenderWindow* window;

	int pheromoneCounter = 0;
	float elapsedTime;
	int pheromoneSize;
	float resolutionMultiplier;
	int worldSizeX, worldSizeY, windowSizeX, windowSizeY;
	sf::Texture pheromoneTexture;
	sf::Image pheromoneImage;
	sf::Sprite pheromoneSprite;

	sf::Clock clock;


	std::vector< sf::CircleShape > antShapes;
	std::vector< sf::CircleShape > jobShapes;

	std::vector< sf::CircleShape > antSensorLeft;
	std::vector< sf::CircleShape > antSensorRight;



	void drawPheromones();
	void drawAnts();
	void drawJobs();
	void ajustAntSize(int newSize);

};

