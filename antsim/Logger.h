#pragma once
#include <fstream>

#include "Core.h"
#include "TaskList.h"

class Logger
{

public:


	enum LogFile{__FirstFake__, FilePathLengths, FileJobAllocation, FileJobsCompleted, __LastFake__};

	std::string logFilePath(LogFile logFile)
	{
		std::string path = "Logs/";
		switch (logFile)
		{
		case FilePathLengths:
			path += "PathLengths";
			break;
		case FileJobAllocation:
			path += "JobAllocation";
			break;
		case FileJobsCompleted:
			path += "JobsCompleted";
			break;
		
		}


		switch (ANT_CONTROLLER)
		{
		case 0:
			path += "_Pheromone";
			break;
		case 1:
			path += "_CentralControl";
			break;
		}
		path += ".csv";
		return path;
	}

	void ensureFile(LogFile logFile)
	{
		std::string path= logFilePath(logFile);
		std::ofstream myfile(path);
		if (myfile.is_open())
		{
			myfile.close();
		}else
		{
			throw(path + " Could not be created");
		}
	}
	void writeLineFile(LogFile logFile, std::string output)
	{
		output += "\n";



		std::string path = logFilePath(logFile);
		std::ofstream myfile(path, std::ios::app | std::ios::out);
		if (myfile.is_open())
		{
			myfile << output;
			myfile.close();
		}
	}

	explicit Logger(Core* core)
		: core(core)
	{
		for (int logFile = __FirstFake__+1; logFile != __LastFake__; logFile++)
		{
			LogFile log = static_cast<LogFile>(logFile);
			ensureFile(log);
		}
		
		//myfile.open("Output/example.txt", std::ios::trunc|| std::ios::out);
		
	}

	void Update()
	{
		getAntJobsAlocation();
		pathOutput();
		tick++;
		
	}
	void pathOutput()
	{

		std::string output=std::to_string(tick)+"; ";
		auto paths = core->world->pathArchive->getUnreadPaths();
		std::set<int> typeIds = core->world->jobMap->getAllTypeIds();
		
		for (int typeId : typeIds)
		{
			int count = 0;
			float totalLenghts=0;
			for (Path path : (paths)[typeId])
			{
				totalLenghts += path.getLenght();

				count++;
				if (count > 20) break;
			}
			if(count>0)
			{
				totalLenghts /= count;
				output += std::to_string(totalLenghts) + "; ";
			}else
			{
				output += "; ";
			}
		}

		output.pop_back();
		output.pop_back();
		
		writeLineFile(FilePathLengths,output);
		
		
		core->log.push(output);
	}

	void getAntJobsAlocation() {

		std::map<int,int> counts;
		int i = 0;
		for (auto state_name : states::stateNames)
		{
			counts[i++] = 0;
		}

		for (auto ant : core->world->antContainer->ants)
		{
			int state = ant->taskDriver->taskDriver.task->state;
			counts[state]++;
		}
		std::string output;
		for (auto count : counts)
		{
			output += states::stateNames[count.first] + ": " + std::to_string(count.second) + "\t";
		}

		//core->log.push(output);
	}



private:
	int tick = 0;
	

	Core* core;
};

