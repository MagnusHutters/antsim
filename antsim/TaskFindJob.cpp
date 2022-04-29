#include "TaskFindJob.h"

#include <random>
#include <utility>


void TaskFindJob::doStateFindJob(PheromoneId findPheromone, PheromoneId trailPheromone)
{

	state = states::FindJob;
	currentCondition = Conditions(findPheromone);

	action->setAction(new ActionExplore(EXPLORED_PHEROMONE_ID,true,findPheromone));
	pheromone->setTrail(trailPheromone);
	//action->setAction(new ActionFollowTrail(currentPheromone));
	//findPheromone->removeTrails();
	//findPheromone->removeTrail(EXPLORED_PHEROMONE_ID, true);
	//findPheromone->removeTrail(PATH_PHEROMONE_ID, true);
	//action->setAction(new ActionHold());
	


}

void TaskFindJob::stateFindJob()
{
	
	

	/*float isPath = sensor->senseBelow(PATH_PHEROMONE_ID, true, SENSOR_RADIUS_MEDIUM);
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
	counter++;*/

}

void TaskFindJob::doStateExplore(bool setTrailToLastJob, int lastJob, Conditions conditions)
{
	state = states::Explore;
	action->setAction(new ActionExplore({EXPLORED_PHEROMONE_ID,true}));
	pheromone->setTrail({EXPLORED_PHEROMONE_ID,true});


	/*if (setTrailToLastJob)
	{
		pheromone->addTrail(lastJob,true,1,PHEROMONE_DECAY_FAST);
		pheromone->addTrail(PATH_PHEROMONE_ID, true,1,PHEROMONE_DECAY_FAST);
	}*/

	
	currentCondition = std::move(conditions);
	
	//currentCondition = doFocusPheromone ? Conditions()

}

void TaskFindJob::stateExplore()
{ //EXP

	if(findJobPoint(currentCondition))
	{
		doStateFoundJobPoint();
	}

	/*
	//std::array<float, MAX_NUMBER_OF_TASKS> strenghts;
	float maxStrenght = 0;
	int maxStrenghtId = -1;
	float newStrenght = 0;
	if(doFocusPheromone)
	{
		maxStrenght = sensor->senseBelow(currentPheromone, true, SENSOR_RADIUS_HUGE);
		if(maxStrenght> PHEROMONE_FLOOR*50)
		{
			doStateFindJob(currentPheromone);
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
			doStateFindJob(maxStrenghtId);
		}
	}*/


	//checkForJobPoint();

}

void TaskFindJob::doStateFoundJobPoint()
{
	state = states::FoundJobPoint;
	action->setAction(new ActionGoTowardNearbyTask());
	pheromone->setTrail({EXPLORED_PHEROMONE_ID,true});

}


TaskFindJob::_ChoosePrimaryInterfaceOrExploreResult TaskFindJob::ChoosePrimaryInterfaceOrExplore(Job* job)
{
	auto weights = job->getAllJobInterfaceWeights();
	weights.weights.push_back(EXPLORE_WEIGHT);

	std::discrete_distribution<> dist(weights.weights.begin(), weights.weights.end());

	auto var = dist.max();
	int choice = dist(gen);
	bool doExploreResult = choice == weights.pheromones.size();
	PheromoneId pheromone{-1,0};
	if (!doExploreResult) pheromone = weights.pheromones[choice];
	return { doExploreResult,pheromone,static_cast<float>(weights.weights[choice])/WEIGHT_SIZE};
}

void TaskFindJob::stateFoundJobPoint()
{ 

	auto jobResult=sensor->getJob(currentCondition);
	if (!jobResult.success) return;
	Job* job = jobResult.entity;


	auto choice = ChoosePrimaryInterfaceOrExplore(job);
	if(choice.doExploreResult) //Do i go explore
	{
		doStateExplore(currentCondition + Conditions(job->handle));
	}else //Do i go do a job
	{


		//TODO Check if i need to recruit
		float recruitmentStrenght = sensor->senseBelow({RECRUIT_PHEROMONE_ID,true}, SENSOR_RADIUS_LARGE);
		float pheromoneStrenght = sensor->senseBelow(choice.pheromone, SENSOR_RADIUS_LARGE);

		float doRecruitStrenght = (choice.weight * JOB_RECRUITMENT_STRENGHT_MULTIPLIER) / (1 + ((sqrt(pheromoneStrenght+ recruitmentStrenght)) * recruitmentStrenght));
		float dice = randomFloat(gen2);
		if(doRecruitStrenght>dice)
		{
			doStateRecruit();
			
		}else
		{
			doStateExplore(Conditions(job->handle));
		}

	}
	//std::discrete_distribution<> d({ 40, 10, 10, 40 });

	
	/*Job::JobReport jobReport = jobResult->doInteract(body);
	if (jobReport.successful) {

		pheromone->success();
		//jobHandle = jobReport.currentHandle;
		currentPheromone = jobReport.currentPheromone;

		//float trailStrenght = sensor->senseStrenght(jobId, true);
		float successStrenght = sensor->senseStrenght(SUCCESS_PHEROMONE_ID, true);
		if (successStrenght < TASK_SUCCESS_THRESHOLD)
		{
			doStateRecruit(jobReport.nextPartPheromone);
			//nextPheromone = jobReport.nextPartPheromone;
		}
		else
		{
			if (jobReport.isNextPart)
			{
				//report = TaskReport(SimpleJob, jobReport.nextPartHandle);
				//jobHandle = jobReport.nextPartHandle;

				//doStateFindJob(jobReport.nextPartPheromone);
				doStateExplore(true,currentPheromone,true, jobReport.nextPartPheromone);
			}
			else
			{
				doStateExplore(true,currentPheromone);
				//report = TaskReport(FindJob, currentPheromone, jobReport.currentHandle);
				//taskDriver->setTask(new TaskFindJob());
			}
		}

	}*/
}

void TaskFindJob::doStateRecruit(int nextJobPheromone)
{
	state = states::Recruit;
	currentPheromone = { RECRUIT_PHEROMONE_ID,true };
	action->setAction(new ActionExplore(currentPheromone));
	//nextJobPheromone->setTrail(jobId, true);
	pheromone->setTrail(currentPheromone, TASK_SUCCESS_THRESHOLD, PHEROMONE_DECAY_FAST);
	declareStrenght = TASK_SUCCESS_THRESHOLD;

}

void TaskFindJob::stateRecruit()
{
	
	//pheromone->emit(currentPheromone, true, declareStrenght);
	auto pheromoneJob=pheromone->getTrail(currentPheromone);
	if(pheromoneJob.strenght< PHEROMONE_FLOOR)
	{
		
		doStateExplore();
	}
	



}

void TaskFindJob::doStateDecideNextTask()
{
	state = states::DecideNextTask;
	action->setAction(new ActionHold());

	//action->setAction(new ActionFollowTrail(0));
	//nextJobPheromone->setTrail(0);

}

void TaskFindJob::stateDecideNextTask()
{
	/*doStateDecideNextTask();
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
	}*/

}
