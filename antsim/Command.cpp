//#include "Command.h"
//
//#include "Core.h"
//
//
//void help(Core*, std::list<std::string>);
//
//
//Command::Command()
//{
//	
//	
//	
//	
//	std::list<commandStruct> tempCommands;
//	
//	tempCommands.push_back(commandStruct("help", &help, "Prints this"));
//	
//	//tempCommands.push_back(commandStruct(&help));
//
//
//
//
//	for (const auto& command : tempCommands)
//	{
//		commands[command.key] = commandStruct(command.key,command.function,command.description);
//	}
//		
//}
//
//
//void Command::executeCommand(Core* core, list<string> list)
//{
//	std::string commandKey=list.front();
//	list.pop_front();
//
//	std::map<std::string, commandStruct>::iterator it;
//
//
//	it = core->commandExecuter->commands.find(commandKey);
//
//	if (it == core->commandExecuter->commands.end()) {
//		core->log.push("Unkown Command");
//	}
//	else {
//		core->commandExecuter->commands[commandKey].function(core, list);
//	}
//	
//	
//}
//
//
//
//void help(Core* core, std::list<std::string> list)
//{
//
//	core->log.push("================================");
//	std::map<std::string, commandStruct>::iterator it = core->commandExecuter->commands.begin();
//	for (std::pair<std::string, commandStruct> command : core->commandExecuter->commands) {
//		
//		core->log.push(command.second.key+" > "+command.second.description);
//	}
//	core->log.push("================================");
//	
//}
//
//