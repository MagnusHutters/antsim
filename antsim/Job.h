#pragma once




#include "Config.h"
#include "Vector2.h"
#include "EntityMap.h"


class Job
{
	struct JobReport {
		bool successful;
		bool isNextPart;
		int nextPartId;
	};

public:
	//Job();
	Job(EntityMap<Job>* jobMap, Vector2 pos) : jobMap(jobMap), pos(pos), handle(0) {
		
		handle = jobMap->registerEntity(this, pos);
	}

	virtual bool doInteract() {
		return false;
	}

	virtual bool getReady() {
		return false;
	}

private:
	Vector2 pos;
	EntityMap<Job>* jobMap;
	int handle;



};

