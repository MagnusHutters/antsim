#pragma once

#include <queue>
#include <iostream>
#include <string>
#include "Command.h"
#include "World.h"



using namespace std;

class Command;
class World;


class Core
{
public:




	Core() {

	}

	queue<string> log;
	Command* commandExecuter;
	World* world;


private:






};

