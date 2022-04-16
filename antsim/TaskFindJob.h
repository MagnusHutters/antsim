
#pragma once

#include "TaskList.h"
//#include "TaskDriver.h"
//#include "TaskSimpleJob.h"

#include "ActionExplore.h"
#include "ActionHold.h"
#include "ActionFollowTrail.h"
#include "ActionGoTowardNearbyTask.h"
#include "Job.h"



class TaskFindJob : public Task {

public:
    TaskFindJob()
    {}
    TaskFindJob(int avoidHandle)
    {
        avoidJobHandle=avoidHandle;
    }
    

	void init() {
        state = states::Explore;
        pheromone->removeTrails();
	}

    

	TaskReport calcTask() {
        manageMetaData();
        


        switch (state)
        {
        case states::FollowingTrail:
            stateFollowingTrail();
            break;
        case states::Explore:
            stateExplore();
            break;
        case states::FoundJobPoint:
            stateFoundJobPoint();
            break;
        case states::DecideNextTask:
            stateDecideNextTask();
            break;
        case states::LookAround:
            break;
        case states::DeclareJob:
            stateDeclareJob();
            break;
        default:
            state = states::Explore;
            break;
        }

        return report;
	}

    void stateFollowingTrail() {
        if (newState) {
            action->setAction(new ActionFollowTrail(jobPheromone));
            pheromone->removeTrails();
            //action->setAction(new ActionHold());
        }
        if(findJobPoint())
        {
            state = states::FoundJobPoint;
        }
        if(counter>10)
        {
	        
            float strenght = sensor->senseBelow(jobPheromone, true);
            if(strenght<PHEROMONE_FLOOR)
            {
                state = states::Explore;
            }
            counter = 0;
        }
        counter++;

    }
    void stateExplore() {
        if (newState) {
            action->setAction(new ActionExplore(EXPLORED_PHEROMONE_ID));
            pheromone->setTrail(EXPLORED_PHEROMONE_ID);
        }
        if(findJobPoint())
        {
            state = states::FoundJobPoint;
        }
        //std::array<float, MAX_NUMBER_OF_TASKS> strenghts;
        float maxStrenght = 0;
        int maxStrenghtId = -1;
        float newStrenght = 0;
        for (int i = 0; i < MAX_NUMBER_OF_TASKS; ++i)
        {
            newStrenght = sensor->senseBelow(i, true, SENSOR_RADIUS_TINY);
            if(newStrenght>maxStrenght)
            {
                maxStrenght = newStrenght;
                maxStrenghtId = i;
            }
        }
        if(maxStrenght>PHEROMONE_FLOOR)
        {
            state = states::FollowingTrail;
            jobPheromone = maxStrenghtId;
        }

        //checkForJobPoint();
        
    }

    void stateFoundJobPoint() {
        if (newState) {
            action->setAction(new ActionGoTowardNearbyTask());
            pheromone->setTrail(EXPLORED_PHEROMONE_ID);
        }
        Job* job=sensor->getJob(ignoreJobHandle);
       
        Job::JobReport jobReport = job->doInteract(body);
        if (jobReport.successful) {

            pheromone->success();
            jobHandle = jobReport.currentHandle;
            jobPheromone = jobReport.currentPheromone;

            //float trailStrenght = sensor->senseStrenght(jobId, true);
            float successStrenght = sensor->senseStrenght(SUCCESS_PHEROMONE_ID, true);
            if (successStrenght < TASK_SUCCESS_THRESHOLD)
            {
                state = states::DeclareJob;
            }
            else
            {
                if (jobReport.isNextPart)
                {
                    //report = TaskReport(SimpleJob, jobReport.nextPartHandle);
                    jobHandle = jobReport.nextPartHandle;
                    jobPheromone = jobReport.nextPartPheromone;
                }
                else
                {
                    state = states::Explore;
                    //report = TaskReport(FindJob, jobPheromone, jobReport.currentHandle);
                    //taskDriver->setTask(new TaskFindJob());
                }
            }

        }
    }

    void stateDeclareJob()
    {
        if (newState) {
            action->setAction(new ActionExplore(jobPheromone));
            //nextJobPheromone->setTrail(jobId, true);
            declareStrenght = TASK_SUCCESS_THRESHOLD;
        }
        pheromone->emit(jobPheromone, true, declareStrenght);
        declareStrenght *= PHEROMONE_DECAY * PHEROMONE_DECAY * PHEROMONE_DECAY;
        if (declareStrenght < PHEROMONE_FLOOR)
        {
            state = states::FollowingTrail;
        }


    }

    void stateDecideNextTask() {
        if (newState) {
            action->setAction(new ActionHold());
            
            //action->setAction(new ActionFollowTrail(0));
            //nextJobPheromone->setTrail(0);
        }
        if(findJobPoint())
        {

            Job* job = sensor->getJob();
            Job::JobReport jobReport = job->doGetInfo(body);
            if(jobReport.successful)
            {
                report = TaskReport(Tasks::SimpleJob, jobReport.currentPheromone);
                //taskDriver->setTask(new TaskSimpleJob(jobReport.currentPheromone));
            }

        }else
        {
            state = states::Explore;
        }

    }

    //STATES END

	void doTask() {

	}

private:

    int ignoreJobHandle = -1;
    int jobPheromone =-1;
    int jobHandle = -1;
    int counter = 0;
    float declareStrenght = 0.0f;
    
};