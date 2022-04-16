#pragma once

#include <list>



#include "Config.h"
#include "Job.h"
#include "EntityMap.h"

class JobFactory
{
public:
	JobFactory(EntityMap<Job>* jobMap) : jobMap(jobMap) {
		for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++)
		{
			freeJobIds.push_back(i);
		}
	}

	void createRandomSimpleJob(int number = 1) {
		for (int i = 0; i < number; i++)
		{
			Vector2 newPos = Vector2::RandomWithinMap();
			int newId = freeJobIds.front();
			freeJobIds.pop_front();
			Job* job = new Job(jobMap, newPos, newId);
		}
	
	}

private:

	std::list<Job*> jobs;
	EntityMap<Job>* jobMap;
	std::list<int> freeJobIds;


};

