
#pragma once

#include "TaskList.h"
#include "TaskDriver.h"
//#include "TaskSimpleJob.h"

#include <random>

#include "ActionExplore.h"
#include "ActionHold.h"
#include "ActionFollowTrail.h"
#include "ActionGoTowardNearbyTask.h"
#include "Job.h"
#include "Conditions.h"


class TaskFindJob : public Task {

public:
    TaskFindJob()
    {
        
        gen = std::mt19937(rd());
        gen2 = std::mt19937(rd2());
        randomFloat = std::uniform_real_distribution<float>(0.0, 1.0);
    }
    TaskFindJob(int avoidHandle) : TaskFindJob()
    {
        avoidJobHandle = avoidHandle;
    }
    

	void init() {
        doStateExplore();
        
	}

    

	TaskReport calcTask() {
        manageMetaData();
        


        switch (state)
        {
        case states::FindJob:
            stateFindJob();
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
        case states::Recruit:
            stateRecruit();
            break;
        default:
            doStateExplore();;
            break;
        }

        return report;
	}

    void doStateFindJob(PheromoneId findPheromone, PheromoneId trailPheromone);

    void stateFindJob();

    void doStateExplore(Conditions condition=Conditions())
    {
        doStateExplore(false, -1, std::move(condition));
    }



    void doStateExplore(bool setTrailToLastJob, int lastJob, Conditions conditions = Conditions());

    void stateExplore();

    void doStateFoundJobPoint();

    struct _ChoosePrimaryInterfaceOrExploreResult {
        bool doExploreResult; PheromoneId pheromone; float weight;
    };
    _ChoosePrimaryInterfaceOrExploreResult ChoosePrimaryInterfaceOrExplore(Job* job);

    void stateFoundJobPoint();

    void doStateRecruit(int nextJobPheromone=-1);

    void stateRecruit();

    void doStateDecideNextTask();

    void stateDecideNextTask();

    //STATES END

	void doTask() {

	}

private:

    std::random_device rd;
    std::random_device rd2;
    std::mt19937 gen;
    std::mt19937 gen2;
    std::uniform_real_distribution<float> randomFloat;

    std::list<int> ignorePheromones;
    Conditions currentCondition = Conditions();

    bool doFocusPheromone=false;

    //int ignoreJobHandle = -1;
    PheromoneId currentPheromone;
    PheromoneId oldPheromone;
    //int jobHandle = -1;
    int counter = 0;
    float declareStrenght = 0.0f;
    
};
