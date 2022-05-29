#pragma once

#include <SFML/Graphics.hpp>
#include "EntityMap.h"
#include "GraphicJob.h"
#include "Job.h"
#include <algorithm>

#include "Input.h"

class GraphicAnt;

#include "GraphicAnt.h"



class Core;

#include "Core.h"

#include <vector>
#include <unordered_map>

#define FRAME_TIME 60
#define PHEROMONE_UPDATE_RATE 0
#define MASK_SHAPES_SIZE 16

void imageSaveThread(std::string filename, sf::Texture texture);

class Graphic
{

public:

	Graphic();
	Graphic(Core* core, float resolutionMultiplier);
	

	void update(int tick);

private:

	int numAnts = 0;

	std::list<GraphicAnt*> antsGraphics;
	std::list<GraphicJob*> jobGraphics;

	float deltaTime = 30;
	float deltaTimeAvgIndex = 0.1f;

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

	std::vector<sf::RectangleShape> maskShapes;
	std::vector<sf::Text> maskTexts;

	Input* input;

	long pheromoneMask;
	bool redraw = false;


	void drawTerrain();
	//void createColorFromTypeId(double pheromoneStrenght, int id, int offset, float steepness, sf::Color& con);
	void drawPheromones();
	void drawAnts();
	void drawJobs();
	void ajustAntSize(int newSize);

	void drawPaths();

};

