#pragma once

#include <SFML/Graphics.hpp>
//#include "EntityMap.h"
//#include "Job.h"
#include "Ant.h"
#include "TaskList.h"

//class Core;

//#include "Core.h"
#include "GraphicsUtility.h"
#include "World.h"



class GraphicAnt
{
public:
	
	sf::Color colors[6] = {sf::Color::Blue, sf::Color::Cyan, sf::Color::Green , sf::Color::Magenta , sf::Color::Red , sf::Color::Yellow};

	GraphicAnt(int antId, Ant* ant)
		: mainBody(sf::CircleShape(3, 3)),
		  mainBodyOffset(Vector2(0, 0)),
		  antId(antId),
		  //core(core),
		  ant(ant)
	{
		mainBody.setOrigin(3, 3);
		//sprites.push_back(antSprite(Vector2(0, 0)));

		

		
		
		// select the font
		text.setFont(GraphicsUtility::font);

		text.setString(states::stateShorts[0]);
		text.setCharacterSize(30);
		text.setScale(0.1f, 0.1f);
		text.setOrigin(text.getLocalBounds().width, -3);
		text.setFillColor(sf::Color::Black);
		text.setPosition(Vector2());
	}


	void draw(sf::RenderWindow* window)
	{
		int state = ant->getState();
		Body body =ant->getBody();


		mainBody.setFillColor(colors[state % 6]);
		moveShapeToBody(mainBody, body, mainBodyOffset);
		window->draw(mainBody);
		//sf::Color::

		text.setString(states::stateShorts[state]);
		//text.setCharacterSize(30);
		//text.setScale(0.1, 0.1);
		text.setOrigin(text.getLocalBounds().width*0.5, -3);
		//text.setFillColor(sf::Color::Black);
		text.setPosition(body.pos);
		//text.setStyle();
		window->draw(text);


		
	}

	void moveShapeToBody(sf::Shape& shape, Body& body, Vector2 offset)
	{
		//Vector2 newVector = mainBodyOffset;

		shape.setPosition(offset.Rotate(body.rot) + body.pos);
		shape.setRotation(body.rot + 90);
	}
	
	


private:
	//std::list<antSprite> sprites;
	sf::CircleShape  mainBody;
	Vector2 mainBodyOffset;
	sf::Text text;
	
	int antId;

	//Core* world;
	Ant* ant;

};

