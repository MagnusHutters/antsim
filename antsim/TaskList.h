#pragma once
#include <string>

namespace states
{

	static std::string stateNames[9] = { "None",	"FollowingTrail",	"FoundJobPoint",	"DeclareJob",	"Lost", "DeclareFailure",	"Explore",	"DecideNextTask",	"LookAround" };
	static std::string stateShorts[9] = { "non",	"ft",				"job",				"dJb",			"los",	"dfl",				"exp",		"dec",				"lok" };

	enum State {
		None, FollowingTrail, FoundJobPoint, DeclareJob, Lost, DeclareFailure, Explore, DecideNextTask, LookAround
	};
}


namespace tasklist
{
	enum Tasks { None, FindJob, SimpleJob };
	
}


using namespace tasklist;
class TaskReport
{
public:


	bool isTaskDone;
	Tasks nextTask;

	int currentJobPheromone;
	int nextJobPheromone;
	int currentJobHandle;
	int nextJobHandle;
	bool avoidPreviousJob;
	TaskReport()
		: isTaskDone(false),
		nextTask(None),
		currentJobPheromone(-1),
		nextJobPheromone(-1),
		currentJobHandle(-1),
		nextJobHandle(-1),
		avoidPreviousJob(false)
	{
	}
	
	TaskReport(Tasks nextTask, int nextJobPheromone, int currentJobHandle)
		: isTaskDone(true),
		nextTask(nextTask),
		nextJobPheromone(nextJobPheromone),
		currentJobPheromone(-1),
		nextJobHandle(-1),
		currentJobHandle(currentJobHandle),
		avoidPreviousJob(true)
	{
	}
	TaskReport(Tasks nextTask, int pheromone)
		: isTaskDone(true),
		nextTask(nextTask),
		nextJobPheromone(pheromone),
		currentJobHandle(-1),
		avoidPreviousJob(false)
	{
	}
	TaskReport(Tasks nextTask)
		: isTaskDone(true),
		nextTask(nextTask),
		nextJobPheromone(-1),
		currentJobHandle(-1),
		avoidPreviousJob(false)
	{
	}
	
};

