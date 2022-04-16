#pragma once

#include <queue>
#include <iostream>
#include <string>
//#include "Command.h"
#include "World.h"



//using namespace std;

//class Command;
class World;


class Core
{
public:




	Core() : world(nullptr) {

	}

	std::queue<std::string> log;
	//Command* commandExecuter;
	World* world;


private:






};

