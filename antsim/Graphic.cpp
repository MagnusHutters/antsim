#include "Graphic.h"



Graphic::Graphic(Core* core, float resolutionMultiplier) : elapsedTime(0)
{
	this->core = core;
	worldSizeX = core->world->getSizeX();
	worldSizeY = core->world->getSizeY();
	this->resolutionMultiplier = resolutionMultiplier;

	windowSizeX = worldSizeX * resolutionMultiplier;
	windowSizeY = worldSizeY * resolutionMultiplier;



	window = new sf::RenderWindow(sf::VideoMode(windowSizeX, windowSizeY), "window");

	sf::View view(sf::FloatRect(0.f, 0.f, worldSizeX, worldSizeY));
	//view.zoom(3);
	// activate it
	window->setView(view);

	resolutionMultiplier = 1;

	// draw something to that view
	//window.draw(some_sprite);

	// want to do visibility checks? retrieve the view
	//sf::View currentView = window.getView();


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

	deltaTime = ((1 - deltaTimeAvgIndex) * deltaTime) + (deltaTimeAvgIndex * elapsed.asMilliseconds());
	float fps = 1000 / deltaTime;
	core->log.push(std::to_string(fps));

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
	drawJobs();
	//window->draw(shape);
	window->display();
}


void Graphic::drawPheromones()
{
	int pheromone1 = 0;
	int pheromone2 = RECRUIT_PHEROMONE_ID;
	int pheromone3 = 2;

	pheromoneCounter++;
	if (pheromoneCounter > PHEROMONE_UPDATE_RATE) {
		int size = pheromoneSize;
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				float pheromone1Result = core->world->getPheromone(x, y, pheromone1, 1) * 0.3;
				float pheromone2Result = core->world->getPheromone(x, y, pheromone2, 1) * 0.3;
				float pheromone3Result = core->world->getPheromone(x, y, pheromone3, 1) * 0.3;
				

				float value1 = sinh(pheromone1Result) / cosh(pheromone1Result);
				float value2 = sinh(pheromone2Result) / cosh(pheromone2Result);
				float value3 = sinh(pheromone3Result) / cosh(pheromone3Result);
				
				//value3 = Vector2(x - 50, y - 50).GetNormalFunction(5)*100;
				sf::Uint8 pheromone1Color = 255 - (((value1+ value2)*0.5) * 255);
				sf::Uint8 pheromone2Color = 255 - (((value2+ value3)*0.5) * 255);
				sf::Uint8 pheromone3Color = 255 - (((value3+ value1)*0.5) * 255);

				pheromoneImage.setPixel(x, y, sf::Color(pheromone1Color, pheromone2Color, pheromone3Color));
			}
		}
		pheromoneTexture.update(pheromoneImage);
		pheromoneCounter = 0;
	}

	

	
	window->draw(pheromoneSprite);
}

void Graphic::drawAnts()
{

	if (core->world->getNumAnts() != numAnts) {
		ajustAntSize(core->world->getNumAnts());
		numAnts = core->world->getNumAnts();
	}
	for (auto const& antGraphic : antsGraphics) {
		antGraphic->draw(window);
	}
	

	return;



	std::vector<Body> antBodies = core->world->getAntBodies();

	

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

void Graphic::drawJobs()
{
	std::unordered_map<int, Job*> jobs = core->world->getJobPositions();
	if (jobs.size() != jobShapes.size()) {
		jobShapes.resize(jobs.size(), sf::CircleShape(4));
		for (int i = 0; i < jobShapes.size(); i++)
		{
			jobShapes[i].setFillColor(sf::Color::Magenta);
			jobShapes[i].setOrigin(4, 4);
		}
	}
	int i = 0;
	for (std::pair<int, Job*> element : jobs) {
		int x = element.second->pos.x* 1;
		int y = element.second->pos.y* 1;
		jobShapes[i].setPosition(x, y);
		sf::Vector2f vec;

		window->draw(jobShapes[i]);

		i++;
	}
	for (int i = 0; i < jobs.size(); i++)
	{
		//int x = jobs[i]
		//jobShapes[i].setPosition
	}

}

void Graphic::ajustAntSize(int newSize)
{
	antShapes.reserve(newSize);

	int ajustment = newSize - antShapes.size();

	if (ajustment > 0) {
		for (int i = 0; i < ajustment; i++)
		{
			GraphicAnt* newAntGraphic = new GraphicAnt(antsGraphics.size(), core->world->getAnt(antsGraphics.size()));
			antsGraphics.push_back(newAntGraphic);


			//antShapes.push_back(sf::CircleShape(5, 3));
			//antShapes.back().setFillColor(sf::Color::Black);
			//antShapes.back().setOrigin(5,5);

			//antSensorLeft.push_back(sf::CircleShape(SENSOR_RADIUS_MEDIUM * resolutionMultiplier));
			//antSensorLeft.back().setFillColor(sf::Color::Yellow);
			//antSensorLeft.back().setOrigin(SENSOR_RADIUS_MEDIUM * resolutionMultiplier, SENSOR_RADIUS_MEDIUM * resolutionMultiplier);

			//antSensorRight.push_back(sf::CircleShape(SENSOR_RADIUS_MEDIUM * resolutionMultiplier));
			//antSensorRight.back().setFillColor(sf::Color::Cyan);
			//antSensorRight.back().setOrigin(SENSOR_RADIUS_MEDIUM * resolutionMultiplier, SENSOR_RADIUS_MEDIUM * resolutionMultiplier);



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
