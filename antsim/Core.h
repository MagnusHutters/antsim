#pragma once

#include <queue>
#include <iostream>
#include <string>
//#include "Command.h"
#include <set>

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

	std::set<int> pheromonesToDraw{ 0,1,2,3,4,5,6,7,8,9,10 };


private:






};

