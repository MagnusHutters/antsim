#pragma once

#include "TaskDriver.h"
#include "TaskList.h"
#include "TaskPheromoneController.h"
#include "SensorDriver.h"
#include "ActionDriver.h"
#include "Body.h"
#include "PheromoneDriver.h"
#include "TaskCentralControl.h"


class TaskChanger
{
public:
	TaskChanger(SensorDriver* sensor, ActionDriver* action, BodyDriver* body, PheromoneDriver* pheromoneDriver) :
		taskDriver(TaskDriver(sensor,action,body,pheromoneDriver))
	{
		if(ANT_CONTROLLER==0)
		{
			taskDriver.setTask(new TaskPheromoneController());
		}else if(ANT_CONTROLLER==1)
		{
			taskDriver.setTask(new TaskCentralControl());
		}

		
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
					taskDriver.setTask(new TaskPheromoneController(taskReport.currentJobHandle));
				}
				else
				{
					taskDriver.setTask(new TaskPheromoneController());
				}
				break;
			case SimpleJob:
				taskDriver.setTask(new TaskCentralControl());
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

	void trySetJob(int jobHandle)
	{
		taskDriver.trySetJob(jobHandle);
	}

private:
	friend class Logger;
	TaskDriver taskDriver;
};
