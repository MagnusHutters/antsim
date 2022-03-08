#pragma once

#include "commandline.h"
#include "Core.h"
#include <string>
#include <list>
#include <thread>

#include "Command.h"
#include "World.h"
#include "Graphic.h"



class Controller
{


public:


	Controller();

	void start();

	void doConsole();
	void doGraphics();
	void worldThread();


	list<string> seperateCommand(string command);

	void parseCommand(string command);


	void createEvents();



private:
	bool doShutdown = false;
	Commandline* com;

	atomic_bool worldInitializationDone= false;
	std::mutex m;



	Core* core;
	Graphic* window;
	

	void help(Core*, std::list<std::string>)
{
}
};

