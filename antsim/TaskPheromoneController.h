
#pragma once

#include "TaskList.h"
#include "TaskDriver.h"
//#include "TaskCentralControl.h"

#include <random>

#include "ActionExplore.h"
#include "ActionHold.h"
#include "ActionFollowTrail.h"
#include "ActionGoTowardNearbyTask.h"
#include "Job.h"
#include "Conditions.h"


class TaskPheromoneController : public Task {

public:
    TaskPheromoneController();

    TaskPheromoneController(int avoidHandle);


	void init() {
        doStateExplore();
        
	}

    

	TaskReport calcTask() {
        manageMetaData();
        


        switch (state)
        {
        case states::FindJob:
            stateFindTrail();
            break;
        case states::Explore:
            stateExplore();
            break;
        case states::FoundJobPoint:
            stateFoundJobPoint();
            break;
        case states::FollowTrail:
            stateFollowTrail();
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

private:


    void doStateFindTrail(PheromoneId findPheromone, PheromoneId trailPheromone);


    void stateFindTrail();

    void doStateFollowTrail(PheromoneId findPheromone, bool timeoutToExplore = false);

    void stateFollowTrail();

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

    void doStateRecruit(PheromoneId recruitPheromone);

    void stateRecruit();

    

    //STATES END

	void doTask() {

	}

//private:
    friend class Logger;
    std::random_device rd;
    std::random_device rd2;
    std::mt19937 gen;
    std::mt19937 gen2;
    std::uniform_real_distribution<float> randomFloat;

    std::list<int> ignorePheromones;
    //Conditions currentCondition = Conditions();

    bool doFocusPheromone=false;
    int timeout;
    //int ignoreJobHandle = -1;
    PheromoneId currentPheromone;
    PheromoneId oldPheromone;
    //int jobHandle = -1;
    int counter = 0;
    float declareStrenght = 0.0f;
    bool foundTrail;
    bool doTimeoutToExplore;
};
