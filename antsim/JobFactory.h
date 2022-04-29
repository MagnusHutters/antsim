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

	std::vector<Job*> createRandomSimpleJob(int number = 1);

	int getFreePheromone()
	{
		int newId = freeJobIds.front();
		freeJobIds.pop_front();
		return newId;
	}

	void createJobChain(int number = 2, bool loop=false)
	{

		//Vector2 newPos = Vector2::RandomWithinMap() * 0.5;
		//newPos += Vector2(MAP_WIDTH * 0.25, MAP_HEIGHT * 0.25);
		Vector2 newPos(100, 100);
		Vector2 newPos2(175, 175);

		int newId = getFreePheromone();

		Job* newJob = new Job(newPos, newId, true);
		jobMap->registerEntity(newJob, newPos);

		Job* newJob2 = new Job(newPos2, newId, false);
		jobMap->registerEntity(newJob2, newPos2);
		//jobList.push_back(newJob);

		/*std::vector<Job*> jobList=createRandomSimpleJob(number);

		for (int i = 0; i < number - 1; i++)
		{
			jobList[i]->setNextJob(jobList[i + 1]->handle, jobList[i + 1]->pheromoeId);


		}
		if(loop)
		{
			jobList[number-1]->setNextJob(jobList[0]->handle, jobList[0]->pheromoeId);
		}*/
		

	}


private:

	std::list<Job*> jobs;
	JobMap* jobMap;
	std::list<int> freeJobIds;


};

