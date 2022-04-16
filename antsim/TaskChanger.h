#pragma once

#include "TaskDriver.h"
#include "TaskList.h"
#include "TaskFindJob.h"
#include "TaskSimpleJob.h"
#include "SensorDriver.h"
#include "ActionDriver.h"
#include "Body.h"
#include "PheromoneDriver.h"


class TaskChanger
{
public:
	TaskChanger(SensorDriver* sensor, ActionDriver* action, BodyDriver* body, PheromoneDriver* pheromoneDriver) :
		taskDriver(TaskDriver(sensor,action,body,pheromoneDriver))
	{
		taskDriver.setTask(new TaskFindJob());
	}

	inline void calcTask()
	{
		TaskReport taskReport = taskDriver.calcTask();
		if(taskReport.isTaskDone)
		{
			switch (taskReport.nextTask)
			{
			case FindJob:
				if(taskReport.avoidPreviousJob)
				{
					taskDriver.setTask(new TaskFindJob(taskReport.currentJobHandle));
				}
				else
				{
					taskDriver.setTask(new TaskFindJob());
				}
				break;
			case SimpleJob:
				taskDriver.setTask(new TaskSimpleJob(taskReport.nextJobPheromone));
				break;
			default:
				break;
			}
		}
	}
	inline void doTask() { taskDriver.doTask(); }
	inline int getState()
	{
		return taskDriver.getState();
	}

private:
	TaskDriver taskDriver;
};
