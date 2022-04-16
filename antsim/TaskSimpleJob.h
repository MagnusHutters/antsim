
#pragma once




#include "TaskDriver.h"

//#include "TaskFindJob.h"
#include "TaskList.h"

#include "ActionExplore.h"
#include "ActionHold.h"
#include "ActionFollowTrail.h"
#include "ActionGoTowardNearbyTask.h"
#include "Job.h"


class TaskSimpleJob : public Task {

public:
    

	TaskSimpleJob(int jobId)
		: jobId(jobId)
		  
	{}


	void init() {
        state = states::Lost;
        pheromone->removeTrails();
    }


	

    TaskReport calcTask() override
    {
        manageMetaData();



        switch (state)
        {
        case states::FollowingTrail:
            stateFollowingTrail();
            break;
        case states::FoundJobPoint:
            stateFoundJobPoint();
            break;
        case states::DeclareJob:
            stateDeclareJob();
            break;
        case states::Lost:
            stateLost();
            break;
        default:
            break;
        }
        return report;
    }

    void stateFollowingTrail() {
        if (newState) {
            action->setAction(new ActionExplore(jobId));
            pheromone->removeTrails();
        }
        

    }
    void stateFoundJobPoint()
	{
        if (newState) {
            action->setAction(new ActionGoTowardNearbyTask());
        }

        Job::JobReport jobReport = sensor->getJob()->doInteract(body);
        if(jobReport.successful) //Done Job
        {
            pheromone->success();
            
            //float trailStrenght = sensor->senseStrenght(jobId, true);
            float successStrenght = sensor->senseStrenght(SUCCESS_PHEROMONE_ID, true);
            if(successStrenght < TASK_SUCCESS_THRESHOLD)
            {
                state = states::DeclareJob;
            }else
            {
	            if(jobReport.isNextPart)
	            {
                    report = TaskReport(SimpleJob, jobReport.nextPartHandle);
	            }else
	            {
                    //state=states::
                    report = TaskReport(FindJob, jobId, jobReport.currentHandle);
                    //taskDriver->setTask(new TaskFindJob());
	            }
            }
        }

	}
    void stateDeclareJob()
	{
        if (newState) {
            action->setAction(new ActionExplore(jobId));
            //nextJobPheromone->setTrail(jobId, true);
            declareStrenght = TASK_SUCCESS_THRESHOLD;
        }
        pheromone->emit(jobId, true, declareStrenght);
        declareStrenght *= PHEROMONE_DECAY * PHEROMONE_DECAY* PHEROMONE_DECAY;
        if(declareStrenght<PHEROMONE_FLOOR)
        {
            state = states::FollowingTrail;
        }


	}
    void stateLost()
	{
        if (newState) {
            action->setAction(new ActionHold());
        }
        if(findJobPoint())
        {
            Job::JobReport jobReport = sensor->getJob()->doGetInfo(body);
            if(jobReport.currentPheromone == jobId)
            {
                state = states::FoundJobPoint;
            }
            
        }
	}

    //STATES END

    
    void doTask() {

    }

private:

    int jobId;
    float declareStrenght = 0.0f;
    
};