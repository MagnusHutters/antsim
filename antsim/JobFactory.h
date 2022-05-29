#pragma once

#include <set>
#include <vector>



#include "Config.h"
#include "Job.h"
#include "JobMap.h"
#include "ObstacleMap.h"

class JobFactory
{
public:
	JobFactory(MapContainer m) : m(m) {
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

	std::set<int> createDynamicSuply(int number)
	{
		for (int i = 0; i < number; ++i)
		{
			int toSuply = getFreePheromone();
			keepSuplied.insert(toSuply);

			m.jobMap->registerEntity(Job::createSource(getValidJobPos(), toSuply, false));

		}
		return keepSuplied;
	}


	int extractRandomFromSet(std::set<int>& set) {
		auto randomIndex = rand() % set.size();
		auto it = std::begin(set);
		// 'advance' the iterator n times
		std::advance(it, randomIndex);

		int result = *it;
		set.erase(it);
		return result;
	}

	Vector2 getValidJobPos()
	{
		int minimumDistance = JOB_MINIMUM_DISTANCE;
		while(1)
		{
			
			Vector2 newVector = Vector2::RandomWithinMap();

			if(m.obstacleMap->isObstructed(newVector)) continue; //Retry if in terrain
			if (m.obstacleMap->isObstructed(Vector2( newVector.x,newVector.y+OBSTACLE_CELL_SIZE  ))) continue;
			if (m.obstacleMap->isObstructed(Vector2( newVector.x,newVector.y- OBSTACLE_CELL_SIZE ))) continue;
			if (m.obstacleMap->isObstructed(Vector2( newVector.x+ OBSTACLE_CELL_SIZE,newVector.y ))) continue;
			if (m.obstacleMap->isObstructed(Vector2( newVector.x- OBSTACLE_CELL_SIZE,newVector.y ))) continue;


			minimumDistance--;
			auto result = m.jobMap->getClosest(newVector);
			if (result.success)
			{
				if(result.entity->pos.Distance(newVector) < minimumDistance) continue; //Retry if close to another job
			}
			

			return newVector;
		}
	}

	void createDynamicSuplyChainPyramid(int bottomLayer)
	{
		srand(RANDOM_SEED+50);
		std::set<int> haveSuply = createDynamicSuply(bottomLayer);

		while(haveSuply.size()>1)
		{
			int newPheromone = getFreePheromone();

			//Vector2 newPos = Vector2::RandomWithinMap();
			Vector2 newPos = getValidJobPos();

			//Vector2 newPos = Vector2::RandomWithinMap() * 0.5;
			//newPos += Vector2(MAP_WIDTH * 0.25, MAP_HEIGHT * 0.25);

			int input1 = extractRandomFromSet(haveSuply); // not _really_ random
			int input2 = extractRandomFromSet(haveSuply);

			m.jobMap->registerEntity(Job::createIntermediary(newPos, input1, input2, newPheromone));

			haveSuply.insert(newPheromone);

		}
		int lastPheromone = *(haveSuply.begin());
		m.jobMap->registerEntity(Job::createSink(getValidJobPos(), lastPheromone, true));


	}


	void createJobChain(int number = 2, bool loop=false)
	{

		
		Vector2 newPos = getValidJobPos();
		Vector2 newPos2 = getValidJobPos();
		Vector2 newPos3 = getValidJobPos();
		Vector2 newPos4 = getValidJobPos();

		int pheromone1 = getFreePheromone();
		int pheromone2 = getFreePheromone();
		int pheromone3 = getFreePheromone();

		
		m.jobMap->registerEntity(Job::createSource(newPos, pheromone1, true,0.02,0));
		m.jobMap->registerEntity(Job::createSource(newPos4, pheromone2, true,0.02,0));

		m.jobMap->registerEntity(Job::createIntermediary(newPos2, pheromone1, pheromone2, pheromone3));

		m.jobMap->registerEntity(Job::createSink(newPos3, pheromone3, true));
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

	void assureSuply(int toSuply)
	{
		std::unordered_map<int, Job*> allJobs = m.jobMap->getEntities();

		PheromoneId toSuplyPheromone{ toSuply ,true };
		bool foundFunctionalInterface = false;


		for (auto job : allJobs)
		{
			auto toCheckInterface = job.second->getInterface(toSuplyPheromone);
			if(toCheckInterface.sucess)
			{
				if(toCheckInterface.interface->amount>0)
				{
					foundFunctionalInterface = true;
				}else
				{
					m.jobMap->deleteEntity(job.first);
				}
			}
		}
		if(!foundFunctionalInterface)
		{
			m.jobMap->registerEntity(Job::createSource(getValidJobPos(), toSuply, false));
		}

	}

	void update()
	{
		for (int suplied : keepSuplied)
		{
			assureSuply(suplied);
		}

	}


private:

	std::list<Job*> jobs;
	MapContainer m;
	std::list<int> freeJobIds;

	std::set<int> keepSuplied;


};

