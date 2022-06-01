#pragma once
#include <fstream>

#include "Core.h"
#include "TaskList.h"

class Logger
{

public:


	enum LogFile{__FirstFake__, FilePathLengths, FileJobAllocation, FileJobsCompleted, FileJobFirstPathTime, FileJobFirstInteraction, __LastFake__};

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
		case FileJobFirstPathTime:
			path += "JobFirstPathTime";
			break;
		case FileJobFirstInteraction:
			path += "JobFirstInteraction";
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
		core->log.push(std::to_string(tick));
		paths = core->world->pathArchive->getUnreadPaths();


		getAntJobsAlocation();
		pathOutput();
		pathFirstCompleted();
		jobAllocation();
		jobCompletions();
		tick++;
		
	}

	void jobCompletions()
	{

		std::string output = std::to_string(tick) + "; ";
		auto jobs = core->world->jobMap->entityList;
		for (auto job : jobs)
		{
			if(!job.second->processes.empty())
			{
				output += std::to_string(job.second->processes.front()->getTotal()) + "; ";
			}
		}

		output.pop_back();
		output.pop_back();
		writeLineFile(FileJobsCompleted, output);

	}

	void jobAllocation()
	{
		std::set<int> typeIds = core->world->jobMap->getAllTypeIds();
		std::unordered_map<int, int> antsAllocated;
		for (int type_id : typeIds)
		{
			
		}



		for (auto ant : core->world->antContainer->ants)
		{
			auto task = ant->taskDriver->taskDriver.task;
			if(task->currentCondition.pheromoneWhitelist.empty()) continue;
			if(task->state==states::Deliver || task->state == states::FindJob|| task->state == states::FollowTrail)
			{
				antsAllocated[task->currentCondition.pheromoneWhitelist.front().id]+=1;
			}
			
		}


		std::string output = std::to_string(tick)+"; ";
		for (auto allocated : antsAllocated)
		{
			Vector2 end1, end2;
			auto allJobs = core->world->jobMap->entityList;
			for (auto job : allJobs)
			{
				if(job.second->getInterface({allocated.first,true}).sucess)
				{
					end1 = job.second->pos;
				}
				if (job.second->getInterface({ allocated.first,false }).sucess)
				{
					end2 = job.second->pos;
				}
			}
			output += std::to_string(allocated.second) + "; "+ std::to_string(end1.Distance(end2))+ "; "+ std::to_string(((float)allocated.second) / (float)end1.Distance(end2)) + "; ";;
		}
		output.pop_back();
		output.pop_back();

		writeLineFile(FileJobAllocation, output);
	}

	void pathFirstCompleted()
	{
		bool haveNew = false;

		

		auto& allJobs=core->world->jobMap->entityList;
		for (auto job : allJobs)
		{

			int numberOfInteractions = job.second->interactions;
			if(numberOfInteractions>=0)
			{
				int oldNumberOfInteractions = jobInteractions[job.second->handle];
				if (oldNumberOfInteractions != numberOfInteractions)
				{

					if(oldNumberOfInteractions<1 && numberOfInteractions>=1)
					{
						firstInteraction[job.second->handle] = tick;
					}
					if (oldNumberOfInteractions < 10 && numberOfInteractions >= 10)
					{
						//firstInteraction[job.second->handle] = tick;
						int timeTo1 = firstInteraction[job.second->handle] - jobCreationTime[job.second->handle];
						int timeTo10 = tick - jobCreationTime[job.second->handle];

						
						std::string output = std::to_string(tick) + "; " + std::to_string(timeTo1) + "; " + std::to_string(timeTo10);
						writeLineFile(FileJobFirstInteraction, output);
					}

				}


			}
			jobInteractions[job.second->handle] = numberOfInteractions;




			if(firstPath_allAllJobHanlde.find(job.second->handle)==firstPath_allAllJobHanlde.end())
			{
				firstPath_allAllJobHanlde.insert(job.second->handle);
				firstPath_activeJobHanlde.insert(job.second->handle);
				jobCreationTime[job.second->handle] = tick;
			}
		}

		for (auto path : paths)
		{
			for (auto item : path.second)
			{
				int number = firstPath_activeJobHanlde.erase(item.handle);
				if(number)
				{
					int newTick = tick;
					int oldTick = jobCreationTime[item.handle];

					int tickDifference = newTick - oldTick;
					haveNew = true;
					std::string output = std::to_string(tick) + "; " + std::to_string(tickDifference) + "; " +std::to_string(item.getLenght());
					writeLineFile(FileJobFirstPathTime,output);
				}
			}
			
		}
	}


	void pathOutput()
	{

		std::string output=std::to_string(tick)+"; ";
		
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

	std::unordered_set<int> firstPath_allAllJobHanlde;
	std::unordered_set<int> firstPath_activeJobHanlde;
	std::map<int, int> jobCreationTime;

	std::map<int, int> firstInteraction;

	std::unordered_map<int, int> jobInteractions;

	std::unordered_map<int, std::list<Path>> paths;
	Core* core;
};

