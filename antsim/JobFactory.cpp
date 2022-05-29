#include "JobFactory.h"

std::vector<Job*> JobFactory::createRandomSimpleJob(int number)
{

	/*
	std::vector<Job*> jobList;
	jobList.reserve(number);
	for (int i = 0; i < number; i++)
	{
		Vector2 newPos = Vector2::RandomWithinMap()*0.5;
		newPos += Vector2(MAP_WIDTH * 0.25, MAP_HEIGHT * 0.25);
		int newId = freeJobIds.front();
		freeJobIds.pop_front();
		Job* newJob = new Job(newPos, newId, true);
		jobMap->registerEntity(newJob, newPos);
		jobList.push_back(newJob);
	}
	return jobList;*/
	return {};
}
