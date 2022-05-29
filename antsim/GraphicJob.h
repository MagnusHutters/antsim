#pragma once




#include <SFML/Graphics.hpp>
//#include "EntityMap.h"
//#include "Job.h"
#include "Job.h"


//class Core;

//#include "Core.h"
//#include "World.h"



class GraphicJob
{
public:

	sf::Color colors[6] = { sf::Color::Blue, sf::Color::Cyan, sf::Color::Green , sf::Color::Magenta , sf::Color::Red , sf::Color::Yellow };

	GraphicJob(Job* job)
		: mainBody(sf::CircleShape(4, 4)),
		mainBodyOffset(Vector2(0, 0)),
		antId(antId),
		//core(core),
		job(job)
	{
		//mainBody.setOrigin(3, 3);
		//sprites.push_back(antSprite(Vector2(0, 0)));


		jobHandle = job->handle;

		if (!font.loadFromFile("arial.ttf"))
		{
			// error...
			while (1)
			{

			}
		}
		// select the font
		text.setFont(font);

		text.setString("");
		text.setCharacterSize(30);
		text.setScale(0.1f, 0.1f);
		text.setOrigin(text.getLocalBounds().width, -3);
		text.setFillColor(sf::Color::Black);
		text.setPosition(Vector2());

		mainBody.setFillColor(sf::Color::Magenta);
		mainBody.setOrigin(4, 4);
	}


	void draw(sf::RenderWindow* window)
	{
		//int state = ant->getState();
		//Body body = ant->getBody();

		
		//moveShapeToBody(mainBody, body, mainBodyOffset);
		mainBody.setPosition(job->pos);
		window->draw(mainBody);
		//sf::Color::
		std::string string;
		string += std::to_string(job->handle) + ": (";
		auto amounts = job->getAmounts();
		for (float amount : amounts)
		{
			string += std::to_string(amount) + ", ";
		}
		string.pop_back();
		string.pop_back();
		string += ")";

		text.setString(string);
		//text.setCharacterSize(30);
		//text.setScale(0.1, 0.1);
		text.setOrigin(text.getLocalBounds().width * 0.5f, -3);
		//text.setFillColor(sf::Color::Black);
		text.setPosition(job->pos);
		//text.setStyle();
		window->draw(text);



	}

	void moveShapeToBody(sf::Shape& shape, Body& body, Vector2 offset)
	{
		//Vector2 newVector = mainBodyOffset;

		shape.setPosition(offset.Rotate(body.rot) + body.pos);
		shape.setRotation(body.rot + 90);
	}



	int jobHandle;
private:
	//std::list<antSprite> sprites;
	sf::CircleShape  mainBody;
	Vector2 mainBodyOffset;
	sf::Text text;
	sf::Font font;
	int antId;

	//Core* world;
	Job* job;

};


