#pragma once

#include <vector>



#include "Config.h"
#include "Job.h"
#include "JobMap.h"

class JobFactory
{
public:
	JobFactory(JobMap* jobMap) : jobMap(jobMap) {
		for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++)
		{
			freeJobIds.push_back(i);
		}
	}

	std::vector<Job*> createRandomSimpleJob(int number = 1) {

		std::vector<Job*> jobList;
		jobList.reserve(number);
		for (int i = 0; i < number; i++)
		{
			Vector2 newPos = Vector2::RandomWithinMap()*0.5;
			newPos += Vector2(MAP_WIDTH * 0.25, MAP_HEIGHT * 0.25);
			int newId = freeJobIds.front();
			freeJobIds.pop_front();
			Job* newJob = new Job(newPos, newId);
			jobMap->registerEntity(newJob, newPos);
			jobList.push_back(newJob);
		}
		return jobList;
	}

	void createJobChain(int number = 2, bool loop=false)
	{
		std::vector<Job*> jobList=createRandomSimpleJob(number);

		for (int i = 0; i < number - 1; i++)
		{
			jobList[i]->setNextJob(jobList[i + 1]->handle, jobList[i + 1]->pheromoeId);


		}
		if(loop)
		{
			jobList[number-1]->setNextJob(jobList[0]->handle, jobList[0]->pheromoeId);
		}
		

	}


private:

	std::list<Job*> jobs;
	JobMap* jobMap;
	std::list<int> freeJobIds;


};

