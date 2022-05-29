#pragma once
#include <set>
#include <math.h>


#include "AntContainer.h"
#include "Ant.h"

class CentralController
{


	struct TypePriority
	{
		float totalPriority = 0;
		float outputPriority=0;
		float inputPriority=0;
		Vector2 pos=Vector2();
		int jobHandle=-1;
		

		float getPriority()
		{
			totalPriority = std::min(outputPriority, inputPriority)+2;
			return totalPriority;
		}
	};

	
	struct JobPriorityObject
	{
		JobPriorityObject(const Vector2& jobPos, const Vector2& antPos, int jobHandle, float level, Ant* ant, int typeId)
			: jobPos(jobPos),
			  antPos(antPos),
			  distance(jobPos.Distance(antPos)),
			  jobHandle(jobHandle),
			  ant(ant), 
			  typeId(typeId)
		{
			if (distance == 0.0f)distance = 0.1f;
			priority = sqrtf(level) / distance;
		}

		Vector2 jobPos;
		Vector2 antPos;
		float distance;
		int jobHandle;
		Ant* ant;
		int typeId;

		float priority;


		bool operator<(const JobPriorityObject& jobPriorityObject) const
		{
			return priority > jobPriorityObject.priority;
		}
	};

public:
	CentralController(AntContainer* ants, JobMap* jobMap)
		: ants(ants),
		  jobMap(jobMap)
	{
	}


	
	void update()
	{
		if(counter++<5)
		{
			return;
		}
		counter = 0;

		auto jobs=jobMap->getEntities();

		std::list<Ant*> availableAnts = ants->getAvailableAnts();

		//std::list<TypePriority> outputs;
		std::unordered_map<int, TypePriority> priorityStrenghts;
		
		for (auto job : jobs)
		{
			auto interfaces = job.second->getInterfaces();

			for (auto interface : interfaces)
			{
				if(interface->positive)//Is Source
				{
					priorityStrenghts[interface->typeId].pos = job.second->pos;
					priorityStrenghts[interface->typeId].jobHandle = job.second->handle;
					priorityStrenghts[interface->typeId].outputPriority = interface->amount;
				}else //Is Sink
				{
					priorityStrenghts[interface->typeId].inputPriority = JOB_MAX_AMOUNT - interface->amount;
				}
				
				
			}

			/*float outputPriority = job.second->getOutputLevel();
			if(outputPriority>0)
			{
				outputs.push_back({ outputPriority,job.second->pos,job.second->handle });
			}*/
		}

		std::list<JobPriorityObject> priorityList;

		for (std::pair<const int, TypePriority>& output : priorityStrenghts)
		{
			for (auto ant : availableAnts)
			{
				priorityList.emplace_back(output.second.pos, ant->getBody().pos, output.second.jobHandle, output.second.getPriority(), ant,output.first);
			}
		}
		priorityList.sort();
		std::set<Ant*> doneAnts;

		for (auto priority : priorityList)
		{
			if(doneAnts.find(priority.ant)==doneAnts.end())
			{
				if(priorityStrenghts[priority.typeId].totalPriority>0)
				{
					priorityStrenghts[priority.typeId].totalPriority-=1.0f;
					doneAnts.insert(priority.ant);
					priority.ant->trySetJob(priority.jobHandle);
				}
			}
		}
		availableAnts.clear();
		doneAnts.clear();
		priorityStrenghts.clear();
		
	}

private:
	int counter = 5;
	AntContainer* ants;
	JobMap* jobMap;

};

