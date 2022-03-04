#pragma once


#include <string>
#include <map>
#include <list>

#include "Core.h"


class Core;

struct commandStruct {
	std::string key;
	void (*function)(Core*, std::list<std::string>);
	std::string description;

	commandStruct(std::string key, void (*function)(Core*, std::list<std::string>), std::string description) {
		this->key = key;
		this->function = function;
		this->description = description;
	}
	commandStruct() {
		//this->key = key;
		//this->function = function;
		//this->description = description;
	}


};


class Command
{

public:
	Command();

	void executeCommand(Core*, std::list<std::string>);
	std::map<std::string, commandStruct> commands;


private:

	


	
	

	 // function pointer type
	

	//=============COMMANDS=====================


};

