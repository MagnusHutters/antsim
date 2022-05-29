#pragma once
#include <string>
#include <vector>

namespace states
{

	static std::vector<std::string> stateNames = { "None",	"FindJob",	"FoundJobPoint",	"Recruit",	"Explore",	"FollowTrail",	"LookAround", "Deliver"};
	static  std::vector<std::string>  stateShorts = { "non",	"fj",		"job",				"rcrt",		"exp",		"flw",			"lok", "del"};

	enum State {
		None, FindJob, FoundJobPoint, Recruit, Explore, FollowTrail, LookAround, Deliver
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

	int currentJobPheromone=-1;
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

