#pragma once




#include "Config.h"
#include "Vector2.h"
#include "EntityMap.h"
#include "Body.h"


class Job
{
public:
	struct JobReport {

		bool successful;
		bool isNextPart;
		int currentPheromone;
		int currentHandle;
		int nextPartHandle;
		int nextPartPheromone;
		JobReport() :
			successful(false),
			isNextPart(false),
			currentPheromone(-1),
			nextPartHandle(-1)
		{}

		JobReport(Job* job) :
			successful(true),
			isNextPart(job->haveNextJob),
			currentPheromone(job->pheromoeId),
			currentHandle(job->handle),
			nextPartHandle(job->nextJobHandle),
			nextPartPheromone((job->nextJobPheromone))

		{}

		
	};

	//Job();
	Job(EntityMap<Job>* jobMap, Vector2 pos, int id) : jobMap(jobMap), pos(pos), pheromoeId(id), handle(0) {
		
		handle = jobMap->registerEntity(this, pos);
	}


	virtual const JobReport& doInteract(BodyDriver* body) final{
		if(body->body.pos.Distance(pos)>JOB_INTERACT_RADIUS) { // Out of range
			return JobReport();
		}
		if (!haveNextJob) {
			JobReport(this);
		}
		
	}

	virtual const JobReport& doGetInfo(BodyDriver* body) final {
		if (body->body.pos.Distance(pos) > JOB_INTERACT_RADIUS) { // Out of range
			return JobReport();
		}
		if (!haveNextJob) {
			JobReport(this);
		}

	}

	//virtual bool getReady() {
	//	return false;
	//}

private:
	int pheromoeId;
	int nextJobHandle = -1;
	bool haveNextJob = false;
	Vector2 pos;
	EntityMap<Job>* jobMap;
	int handle;
	int nextJobPheromone=-1;
};

