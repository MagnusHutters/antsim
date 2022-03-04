#include "Controller.h"
#include <SFML/Graphics.hpp>
#include "event.h"
#include "commandline.h"
#include <queue>
#include "Core.h"
#include <string>
#include "commonFuctions.h"

#include "Command.h"



#include <iostream>
#include <sstream>
#include <iterator>
#include <list>
#include <algorithm>
#include "World.h"






Controller::Controller()
{
	core = new Core();
	com = new Commandline();
	core->commandExecuter = new Command();
	core->world = new World("main", 50, 128,128);

	window = new Graphic(core, 8);



	//core->world->start();



	com->write("0");



}







void Controller::start()
{
	
	
	

	while (doShutdown==false) {

		doConsole();

		core->world->update();
		window->update();



		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
		string message = core->log.front();
		com->write(message);
		core->log.pop();
	}

	//com->write("this is a message written with com.write");


}

void Controller::doGraphics()
{
}




list<string> Controller::seperateCommand(string command) {

	
	char space_char = ' ';
	list<string> words{};

	stringstream sstream(command);
	string word;
	while (std::getline(sstream, word, space_char)) {
		//word.erase(commons::remove_if(word.begin(), word.end(), ispunct), word.end());
		words.push_back(word);
	}

	return words;

}

void Controller::parseCommand(string command)
{

	list<string> words = seperateCommand(command);
	/*for (const auto& str : words) {
		core->log.push(str);
	}*/
	core->commandExecuter->executeCommand(core, words);





}





void Controller::createEvents() {




}


