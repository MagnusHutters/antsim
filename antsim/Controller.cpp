#include "Controller.h"
#include <SFML/Graphics.hpp>
#include "event.h"
#include "commandline.h"
#include <queue>
#include "Core.h"
#include <string>
#include "commonFuctions.h"
#include <time.h>

#include "Command.h"



#include <iostream>
#include <sstream>
#include <iterator>
#include <list>
#include <algorithm>
#include "World.h"


#include "Config.h"









Controller::Controller()
{
	core = new Core();

	srand(time(NULL));


	core->world = new World("main", NUM_ANTS, MAP_WIDTH, MAP_HEIGHT);

	
	com = new Commandline();
	//core->commandExecuter = new Command();
	

	window = new Graphic(core, 3.5);

	




	//core->world->start();



	com->write("0");



}

void Controller::worldThread()
{


	

	


	while (1) {

	}
}





void Controller::start()
{
	
	

	while (doShutdown==false) {



		doConsole();

		core->world->update();
		window->update();



		//std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}


	



}

void Controller::doConsole()
{


	if (com->has_command()) {
		auto command = com->get_command();
		com->write(command);
		parseCommand(command);
	}
	

	while (core->log.size()>0) {
		std::string message = core->log.front();
		com->write(message);
		core->log.pop();
	}
	while (core->world->antLog.size() > 0) {
		std::string message = std::string("[Ant] ") + core->world->antLog.front();
		com->write(message);
		core->world->antLog.pop();
	}
	while (core->world->worldLog.size() > 0) {
		std::string message = std::string("[World] ")+core->world->worldLog.front();
		com->write(message);
		core->world->worldLog.pop();
	}

	//com->write("this is a message written with com.write");


}

void Controller::doGraphics()
{
}






std::list<std::string> Controller::seperateCommand(std::string command) {

	
	char space_char = ' ';
	std::list<std::string> words{};

	std::stringstream sstream(command);
	std::string word;
	while (std::getline(sstream, word, space_char)) {
		//word.erase(commons::remove_if(word.begin(), word.end(), ispunct), word.end());
		words.push_back(word);
	}

	return words;

}

void Controller::parseCommand(std::string command)
{

	std::list<std::string> words = seperateCommand(command);
	/*for (const auto& str : words) {
		core->log.push(str);
	}*/
	//core->commandExecuter->executeCommand(core, words);





}





void Controller::createEvents() {




}


