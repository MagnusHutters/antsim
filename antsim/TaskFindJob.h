
#pragma once

#include "TaskList.h"
//#include "TaskDriver.h"
//#include "TaskSimpleJob.h"

#include "ActionExplore.h"
#include "ActionHold.h"
#include "ActionFollowTrail.h"
#include "ActionGoTowardNearbyTask.h"
#include "Job.h"
#include "Conditions.h"


class TaskFindJob : public Task {

public:
    TaskFindJob()
    {}
    TaskFindJob(int avoidHandle)
    {
        avoidJobHandle=avoidHandle;
    }
    

	void init() {
        doStateExplore();
        
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
            doStateExplore();;
            break;
        }

        return report;
	}

    void doStateFollowingTrail(int newPheromone=-1)
    {
        state = states::FollowingTrail;
        if (newPheromone >= 0)
        {
            currentCondition = Conditions(newPheromone, Whitelist);
			if(currentPheromone!= newPheromone) lastPheromone = currentPheromone;
	        currentPheromone = newPheromone;
        }else
        {
            currentCondition = Conditions();
        }
	    action->setAction(new ActionFollowTrail(currentPheromone));
	    //pheromone->removeTrails();
        pheromone->removeTrail(EXPLORED_PHEROMONE_ID, true);
        pheromone->removeTrail(PATH_PHEROMONE_ID, true);
	    //action->setAction(new ActionHold());
        

    }

    void stateFollowingTrail() {
        if(findJobPoint(currentCondition))
        {
            doStateFoundJobPoint();
        }
        float isPath = sensor->senseBelow(PATH_PHEROMONE_ID, true, SENSOR_RADIUS_MEDIUM);
        if(isPath>PHEROMONE_FLOOR)
        {
            //pheromone->emit(currentPheromone, true);
        }

        if(counter>10)
        {
	        
            float strenght = sensor->senseBelow(currentPheromone, true);
            if(strenght<PHEROMONE_FLOOR)
            {
                doStateExplore();;

            }
            counter = 0;
        }
        counter++;

    }
    void doStateExploreSpecific(int focusPheromone = -1)
    {
        doStateExplore(false, -1, true, focusPheromone);
    }
    

    void doStateExplore(bool setTrailToLastJob=false, int lastJob=-1, bool lookingForSpecific=false, int focusPheromone=-1)
    {
        state = states::Explore;
	    action->setAction(new ActionExplore(EXPLORED_PHEROMONE_ID));
	    pheromone->setTrail(EXPLORED_PHEROMONE_ID);
        if (setTrailToLastJob)
        {
	        pheromone->addTrail(lastJob,true,1,PHEROMONE_DECAY_FAST);
            pheromone->addTrail(PATH_PHEROMONE_ID, true,1,PHEROMONE_DECAY_FAST);
        }

        doFocusPheromone = lookingForSpecific;
        if (focusPheromone != lastPheromone) lastPheromone = currentPheromone;
        currentPheromone = focusPheromone;
        if(doFocusPheromone)
        {
            currentCondition = Conditions(currentPheromone, Whitelist);
        }else
        {
            currentCondition = Conditions();
        }
        //currentCondition = doFocusPheromone ? Conditions()
	    
    }

    void stateExplore() { //EXP
        
        if(findJobPoint(currentCondition))
        {
            doStateFoundJobPoint();
        }
        //std::array<float, MAX_NUMBER_OF_TASKS> strenghts;
        float maxStrenght = 0;
        int maxStrenghtId = -1;
        float newStrenght = 0;
        if(doFocusPheromone)
        {
            maxStrenght = sensor->senseBelow(currentPheromone, true, SENSOR_RADIUS_HUGE);
            if(maxStrenght> PHEROMONE_FLOOR*50)
            {
                doStateFollowingTrail(currentPheromone);
            }


        }else{

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
	            doStateFollowingTrail(maxStrenghtId);
	        }
        }


        //checkForJobPoint();
        
    }

    void doStateFoundJobPoint()
    {
        state=states::FoundJobPoint;
	    action->setAction(new ActionGoTowardNearbyTask());
	    pheromone->setTrail(EXPLORED_PHEROMONE_ID);

    }

    void stateFoundJobPoint() { //
        
        Job* job=sensor->getJob(Conditions(ignorePheromones, Blacklist));
       
        Job::JobReport jobReport = job->doInteract(body);
        if (jobReport.successful) {

            pheromone->success();
            //jobHandle = jobReport.currentHandle;
            currentPheromone = jobReport.currentPheromone;

            //float trailStrenght = sensor->senseStrenght(jobId, true);
            float successStrenght = sensor->senseStrenght(SUCCESS_PHEROMONE_ID, true);
            if (successStrenght < TASK_SUCCESS_THRESHOLD)
            {
                doStateDeclareJob(jobReport.nextPartPheromone);
                //nextPheromone = jobReport.nextPartPheromone;
            }
            else
            {
                if (jobReport.isNextPart)
                {
                    //report = TaskReport(SimpleJob, jobReport.nextPartHandle);
                    //jobHandle = jobReport.nextPartHandle;
                    
                    //doStateFollowingTrail(jobReport.nextPartPheromone);
                    doStateExplore(true,currentPheromone,true, jobReport.nextPartPheromone);
                }
                else
                {
                    doStateExplore(true,currentPheromone);
                    //report = TaskReport(FindJob, currentPheromone, jobReport.currentHandle);
                    //taskDriver->setTask(new TaskFindJob());
                }
            }

        }
    }

    void doStateDeclareJob(int nextJobPheromone=-1)
    {
        state = states::DeclareJob;
	    action->setAction(new ActionExplore(currentPheromone));
	    //nextJobPheromone->setTrail(jobId, true);
        pheromone->setTrail(PATH_PHEROMONE_ID, true, TASK_SUCCESS_THRESHOLD, PHEROMONE_DECAY_FAST);
	    declareStrenght = TASK_SUCCESS_THRESHOLD;
        nextPheromone = nextJobPheromone;
    }

    void stateDeclareJob()
    {

        //doStateDeclareJob();
        if(currentPheromone>NUMBER_OF_PHEROMONE_PAIRS)
        {
            volatile int a =1 + 1;
        }
        pheromone->emit(currentPheromone, true, declareStrenght);
        declareStrenght *= PHEROMONE_DECAY_FAST;
        if (declareStrenght < PHEROMONE_FLOOR)
        {
            //doStateFollowingTrail();
            doStateExploreSpecific(nextPheromone);
        }


    }

    void doStateDecideNextTask()
    {
        state = states::DecideNextTask;
	    action->setAction(new ActionHold());
        
	    //action->setAction(new ActionFollowTrail(0));
	    //nextJobPheromone->setTrail(0);
    
    }

    void stateDecideNextTask() {
        doStateDecideNextTask();
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
            doStateExplore();;
        }

    }

    //STATES END

	void doTask() {

	}

private:

    std::list<int> ignorePheromones;
    Conditions currentCondition = Conditions();

    bool doFocusPheromone=false;

    //int ignoreJobHandle = -1;
    int currentPheromone =-1;
    int nextPheromone = -1;
    int lastPheromone = -1;
    //int jobHandle = -1;
    int counter = 0;
    float declareStrenght = 0.0f;
    
};