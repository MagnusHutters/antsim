
#pragma once




#include "TaskDriver.h"

//#include "TaskPheromoneController.h"
#include "TaskList.h"

#include "ActionExplore.h"
#include "ActionHold.h"
#include "ActionFollowTrail.h"
#include "ActionGoToPoint.h"
#include "ActionGoTowardNearbyTask.h"
#include "Job.h"


class TaskCentralControl : public Task {

public:
    

	TaskCentralControl()
		  
	{}


	void init() {
        state = states::None;
        pheromone->removeTrails();
    }


	

    TaskReport calcTask() override
    {
        manageMetaData();



        switch (state)
        {
        case states::FindJob:
            stateFindJob();
            break;
        case states::Deliver:
            stateDeliver();
            break;
        case states::None:
            stateNone();
            break;
        
        default:
            break;
        }
        return report;
    }

    void trySetJob(int jobHandle)
    {
        if (jobHandle == currentJobHandle && state!=states::None)
        {
	        return;
        }
		doStateFindJob(jobHandle);
        
    }

	

    void doStateFindJob(int jobHandle)
	{
        currentJobHandle = jobHandle;
        state = states::FindJob;
        auto result = sensor->getJobFromHandle(jobHandle);

        targetPos = result.entity->pos;

        action->setAction(new ActionGoToPoint(targetPos));
        currentCondition = Conditions(result.entity->getOutput()->getPheromone());
        
	}

	void stateFindJob() {

        auto result = sensor->getJob(currentCondition);
        if(result.success)
        {
            result.entity->transferFromJob(body, result.entity->getOutput()->getPheromone().id);

            doStateDeliver();
        }
        

    }
    void doStateDeliver()
    {
        state = states::Deliver;
        if(!body->hasPacket)
        {
	        state = states::None;
            return;
        }

        int id = body->packet->getTypeId();
        PheromoneId targetPheromone = { id,false };
        currentCondition = Conditions(targetPheromone);

        auto result = sensor->getJobPheromone(targetPheromone);

        currentJobHandle = result.entity->handle;
        if(result.success)
        {
            action->setAction(new ActionGoToPoint(result.entity->pos));
        }
        



    }
    void stateDeliver()
	{
        auto result = sensor->getJob(currentCondition);
        if (result.success)
        {
            result.entity->transferToJob(body);

            doStateNone();
        }

	}

    void doStateNone()
	{
        currentJobHandle = -10;
        state = states::None;
	}

    void stateNone()
	{
        


	}

    //STATES END

    
    void doTask() {

    }

private:


    Conditions currentCondition;
    Vector2 targetPos;

    int currentJobHandle=-10;
    int targetJobHandle;

    int jobId;
    float declareStrenght = 0.0f;
    
};

