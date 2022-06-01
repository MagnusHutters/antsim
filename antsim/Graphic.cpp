#include "Graphic.h"
#include "GraphicsUtility.h"

#include <set>
#include <thread>


Graphic::Graphic(Core* core, float resolutionMultiplier) : elapsedTime(0)
{
	input = new Input();

	GraphicsUtility* graphics_utility = new GraphicsUtility();
	this->core = core;
	worldSizeX = core->world->getSizeX();
	worldSizeY = core->world->getSizeY();
	this->resolutionMultiplier = resolutionMultiplier;
	

	float aspect = (float)worldSizeX / (float)worldSizeY;
	int windowWidth = aspect * (float)WINDOW_HEIGHT;
	window = new sf::RenderWindow(sf::VideoMode(static_cast<unsigned>(windowWidth), WINDOW_HEIGHT), "antsim");
	window->setPosition({ 1920- windowWidth,0});

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
	pheromoneImage.create(size, size,{0,0,0,0});
	
	pheromoneSprite.setTexture(pheromoneTexture);
	pheromoneSprite.setScale(resolutionMultiplier, resolutionMultiplier);


	for (int i = 0; i < NUMBER_OF_PHEROMONE_PAIRS; ++i)
	{
		maskShapes.push_back(sf::RectangleShape({ MASK_SHAPES_SIZE,MASK_SHAPES_SIZE }));
		maskShapes.back().setPosition(i * (MASK_SHAPES_SIZE + 2) + 2, 2);

		maskTexts.push_back(sf::Text(
			std::string(1.0f, input->keys[i].symbol) + ":" + std::to_string(input->keys[i].id),
			GraphicsUtility::font,30
		));
		maskTexts.back().setPosition(i * (MASK_SHAPES_SIZE + 2) + 2, 2);
		maskTexts.back().setScale(0.3, 0.3);
		maskTexts.back().setFillColor(sf::Color::Black);
		
	}

	//pheromoneTexture.update();

	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	
}

void imageSaveThread(std::string filename, sf::Texture texture)
{
	auto img = texture.copyToImage();
	if (img.saveToFile(filename))
	{
		//std::cout << "screenshot saved to " << filename << std::endl;
	}
}

void Graphic::update(int tick)
{



	if (tick % 100 == 0)
	{

		std::string filename = "Screenshots/screenshot_" + std::to_string(tick) + ".png";

		sf::Texture texture;
		texture.create(window->getSize().x, window->getSize().y);
		texture.update(*window);

		//std::thread thread(imageSaveThread, filename, texture);
		imageSaveThread(filename, texture);
		//thread.detach();
	}

	input->update();

	//pheromoneMask ^= input->getKeys(Input::Down);

	if(input->getKeys(Input::Down))
	{
		pheromoneImage.create(pheromoneSize, pheromoneSize, {0,0,0,0});
		redraw = true;
	}

	
	sf::Time elapsed = clock.restart();

	deltaTime = ((1 - deltaTimeAvgIndex) * deltaTime) + (deltaTimeAvgIndex * elapsed.asMilliseconds());
	float fps = 1000 / deltaTime;
	//core->log.push(std::to_string(fps));

	elapsedTime += elapsed.asMilliseconds();
	if (elapsedTime < FRAME_TIME ) {
		if ((tick + 2) % 100 != 0) {
		}else
		{
			
		return;
		}
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


	drawTerrain();
	drawPheromones();
	drawPaths();
	drawAnts();
	drawJobs();

	//window->draw(shape);

	long tempMask = pheromoneMask;
	if ((tick+2) % 100 != 0) {
		for (int i = 0; i < NUMBER_OF_PHEROMONE_PAIRS; ++i)
		{
			long check = 1 & tempMask;
			tempMask = tempMask >> 1;
			if (check)
			{
				maskShapes[i].setFillColor(sf::Color::Green);
			}
			else
			{
				maskShapes[i].setFillColor(sf::Color::Red);
			}
			window->draw(maskShapes[i]);
			window->draw(maskTexts[i]);

		}
	}


	window->display();

	

}


void Graphic::drawTerrain()
{
	sf::RectangleShape wander({ OBSTACLE_CELL_SIZE,OBSTACLE_CELL_SIZE });
	wander.setFillColor(sf::Color::Black);
	auto map = core->world->getTerrain();
	for (int x = 0; x < OBSTACLE_MAP_WIDTH; ++x)
	{
		for (int y = 0; y < OBSTACLE_MAP_HEIGHT; ++y)
		{
			if(map[x][y])
			{
				wander.setFillColor(sf::Color::Black);
			}else
			{
				wander.setFillColor(sf::Color::White);
			}
			wander.setPosition(x * OBSTACLE_CELL_SIZE, y * OBSTACLE_CELL_SIZE);
			window->draw(wander);
		}
	}
}



void Graphic::drawPheromones()
{



	//auto obstrcuted = core->world->getObstructed();



	pheromoneCounter++;
	if (pheromoneCounter > PHEROMONE_UPDATE_RATE) {
		int pheromone1 = 0;
		int pheromone2 = 0;
		int pheromone3 = RECRUIT_PHEROMONE_ID;
		
		PheromoneActiveReturn activePheromones = core->world->getPheromoneActive(redraw);
		redraw = false;
		

		auto activeIter = activePheromones.toUpdate->begin();

		for (ActivePheromoneInfo cleared_cell : activePheromones.clearedCells)
		{
			pheromoneImage.setPixel(cleared_cell.x, cleared_cell.y, {0,0,0,0});
		}
		//core->log.push(std::to_string(activePheromones.toUpdate->size()));
		while (activeIter != activePheromones.toUpdate->end())
		{
			unsigned long bit = 1 << activeIter->id;
			unsigned long isInMask = bit & pheromoneMask;

			if(isInMask)
			{

				double pheromoneStrenght = std::max((*activeIter->value)-0.1, 0.0) * 0.5;


				int id = activeIter->id;
				int offset = activeIter->positive;
				float steepness = 0.5;
				
				sf::Color con = GraphicsUtility::createColorFromTypeId(id, pheromoneStrenght, offset);
				sf::Color old = pheromoneImage.getPixel(activeIter->x, activeIter->y);
				sf::Color mixedColor = { (uint8_t)((con.r + old.r*3) * 0.25),(uint8_t)((con.g + old.g*3) * 0.25) ,(uint8_t)((con.b + old.b*3) * 0.25) };
				if(old.a==0)
				{
					pheromoneImage.setPixel(activeIter->x, activeIter->y, con);
				}
				else
				{
					pheromoneImage.setPixel(activeIter->x, activeIter->y, mixedColor);
				}

			}
			//pheromoneImage.

			activeIter++;
		}
		

		pheromoneTexture.update(pheromoneImage);
		pheromoneCounter = 0;
		core->world->resetPheromoneToUpdate();
	}
	

	
	window->draw(pheromoneSprite);

	return;

	

	
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
	//jobGraphics.clear();
	int my_var = 3;
	std::set<int> graphicsSet;
	std::set<int> jobsSet;

	for (auto job_graphic : jobGraphics)
	{
		delete(job_graphic);

	}
	jobGraphics.clear();
	
	for (std::pair<int, Job*> element : jobs) {
		jobGraphics.push_back(new GraphicJob(element.second));
	}

	for (GraphicJob* job_graphic : jobGraphics)
	{
		job_graphic->draw(window);
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

void Graphic::drawPaths()
{
	auto paths = core->world->getPaths();

	for (const auto& typeId : *(paths.paths))
	{
		int count = 0;
		for (const auto& path : typeId.second)
		{
			count++;
			if(count>50)
			{
				break;
			}
			window->draw(path.vertexArray);
		}
	}
}
