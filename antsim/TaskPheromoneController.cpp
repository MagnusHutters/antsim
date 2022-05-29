#include "TaskPheromoneController.h"

#include <random>
#include <utility>


TaskPheromoneController::TaskPheromoneController()
{
        
	gen = std::mt19937(rd());
	gen2 = std::mt19937(rd2());
	randomFloat = std::uniform_real_distribution<float>(0.0, 1.0);
}

TaskPheromoneController::TaskPheromoneController(int avoidHandle): TaskPheromoneController()
{
	avoidJobHandle = avoidHandle;
}

void TaskPheromoneController::doStateFindTrail(PheromoneId findPheromone, PheromoneId trailPheromone={EXPLORED_PHEROMONE_ID,true})
{

	state = states::FindJob;
	
	currentCondition = Conditions(findPheromone);
	currentPheromone = findPheromone;
	oldPheromone = trailPheromone;
	

	action->setAction(new ActionExplore(trailPheromone,true,findPheromone));
	if(trailPheromone.id>=0)
	{
		float str=  sensor->senseBelow(trailPheromone.inverse(), SENSOR_RADIUS_LARGE);
		pheromone->setTrail(trailPheromone, (float)(PHEROMONE_TRAIL_START_STRENGTH*2)- str, (float)PHEROMONE_DECAY_FAST);
	}
	//pheromone->addTrail({ EXPLORED_PHEROMONE_ID,true });
	//action->setAction(new ActionFollowTrail(currentPheromone));
	//findPheromone->removeTrails();
	//findPheromone->removeTrail(EXPLORED_PHEROMONE_ID, true);
	//findPheromone->removeTrail(PATH_PHEROMONE_ID, true);
	//action->setAction(new ActionHold());
	


}

void TaskPheromoneController::stateFindTrail()
{
	if(pheromone->getTrail(oldPheromone).strenght<PHEROMONE_FLOOR)
	{
		if(body->hasPacket)
		{
			oldPheromone = { EXPLORED_PHEROMONE_ID,true };
			action->setAction(new ActionExplore(oldPheromone, true, currentPheromone));
			pheromone->setTrail(oldPheromone);
		}else
		{
			doStateExplore();
		}
		
	}

	if (findJobPoint(currentCondition))
	{
		doStateFoundJobPoint();
	}
	float isPath = sensor->senseBelow(currentPheromone, SENSOR_RADIUS_MEDIUM);
	float strenght = action->getAction()->getStrenght();
	if((strenght+ isPath)>PHEROMONE_FLOOR)
	{
		doStateFollowTrail(currentPheromone);
	}

	

	

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

void TaskPheromoneController::doStateFollowTrail(PheromoneId findPheromone, bool timeoutToExplore)
{
	state = states::FollowTrail;
	doTimeoutToExplore = timeoutToExplore;
	timeout = 0;
	currentPheromone = findPheromone;
	action->setAction(new ActionFollowTrail(findPheromone));
	currentCondition = Conditions(currentPheromone);
	pheromone->removeTrail({ EXPLORED_PHEROMONE_ID,true });

	auto currentTrail = pheromone->getTrail(oldPheromone);
	pheromone->setTrail(currentTrail.pheromone, currentTrail.strenght, currentTrail.decay, { 0.0,-3.0 });

	if(findPheromone.id>= MAX_NUMBER_OF_TASKS)
	{
		currentCondition = Conditions();

	}

	//action->setAction(new ActionFollowTrail(0));
	//nextJobPheromone->setTrail(0);

}

void TaskPheromoneController::stateFollowTrail()
{
	if (findJobPoint(currentCondition))
	{
		doStateFoundJobPoint();
	}
	float howLost = action->getAction()->getStrenght();
	if (howLost > 50)
	{
		
		if(doTimeoutToExplore)
		{
			doStateExplore();
		}
		else
		{
			
			doStateFindTrail(currentPheromone);
		}
		counter = 0;
	}

	/*doStateFollowTrail();
	if(findJobPoint())
	{

		Job* job = sensor->getJob();
		Job::JobReport jobReport = job->doGetInfo(body);
		if(jobReport.successful)
		{
			report = TaskReport(Tasks::SimpleJob, jobReport.currentPheromone);
			//taskDriver->setTask(new TaskCentralControl(jobReport.currentPheromone));
		}

	}else
	{
		doStateExplore();;
	}*/

}

void TaskPheromoneController::doStateExplore(bool setTrailToLastJob, int lastJob, Conditions conditions)
{
	state = states::Explore;
	action->setAction(new ActionExplore({EXPLORED_PHEROMONE_ID,true}));
	pheromone->setTrail({EXPLORED_PHEROMONE_ID,true});


	/*if (setTrailToLastJob)
	{
		pheromone->addTrail(lastJob,true,1,PHEROMONE_DECAY_FAST);
		pheromone->addTrail(PATH_PHEROMONE_ID, true,1,PHEROMONE_DECAY_FAST);
	}*/
	counter = 0;
	
	currentCondition = std::move(conditions);
	
	//currentCondition = doFocusPheromone ? Conditions()

}

void TaskPheromoneController::stateExplore()
{ //EXP

	if(findJobPoint(currentCondition))
	{
		doStateFoundJobPoint();
	}
	if(counter++ > JOB_EXPLORE_RECRUIT_LOCKOUT ||(currentCondition.jobBlacklist.size()==0 && currentCondition.pheromoneWhitelist.size()==0))
	{
		
		float strenght = sensor->senseBelow({RECRUIT_PHEROMONE_ID,true},SENSOR_RADIUS_HUGE);

		if (strenght > PHEROMONE_FLOOR * 50)
		{
			doStateFollowTrail({ RECRUIT_PHEROMONE_ID,true } , true);
		}
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
			doStateFindTrail(currentPheromone);
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
			doStateFindTrail(maxStrenghtId);
		}
	}*/


	//checkForJobPoint();

}

void TaskPheromoneController::doStateFoundJobPoint()
{
	state = states::FoundJobPoint;
	action->setAction(new ActionGoTowardNearbyTask());
	//pheromone->setTrail({EXPLORED_PHEROMONE_ID,true});

}


TaskPheromoneController::_ChoosePrimaryInterfaceOrExploreResult TaskPheromoneController::ChoosePrimaryInterfaceOrExplore(Job* job)
{
	auto weights = job->getAllJobInterfaceWeights();
	weights.weights.push_back(EXPLORE_WEIGHT);

	std::discrete_distribution<> dist(weights.weights.begin(), weights.weights.end());

	counter = 0;
	auto var = dist.max();
	int choice = dist(gen);
	bool doExploreResult = choice == weights.pheromones.size();
	PheromoneId pheromone{-1,0};
	if (!doExploreResult) pheromone = weights.pheromones[choice];
	return { doExploreResult,pheromone,static_cast<float>(weights.weights[choice])/WEIGHT_SIZE};
}

void TaskPheromoneController::stateFoundJobPoint()
{ 

	counter++;
	if(counter>10)
	{
		counter = 0;
		if(sensor->senseJob(currentCondition).Length()==0)
		{
			doStateExplore();
		}
	}

	auto jobResult=sensor->getJob(currentCondition);
	if (!jobResult.success) return;
	Job* job = jobResult.entity;

	if(body->hasPacket )
	{
		auto interfaceResult = job->getInterface({ body->packet->getTypeId(), false });
		if(interfaceResult.sucess)
		{
			job->transferToJob(body);
		}
	}


	auto choice = ChoosePrimaryInterfaceOrExplore(job);
	if(choice.doExploreResult) //Do i go explore
	{
		doStateExplore(Conditions(job->handle));
	}else //Do i go do a job
	{


		//TODO Check if i need to recruit
		float recruitmentStrenght = sensor->senseBelow({RECRUIT_PHEROMONE_ID,true}, SENSOR_RADIUS_LARGE);
		float pheromoneStrenght = sensor->senseBelow(choice.pheromone, SENSOR_RADIUS_LARGE);

		float doRecruitStrenght = (choice.weight * JOB_RECRUITMENT_STRENGHT_MULTIPLIER) / (1 + ((sqrt(pheromoneStrenght+ recruitmentStrenght)) * recruitmentStrenght));
		float dice = randomFloat(gen2);
		if(doRecruitStrenght>dice)
		{
			doStateRecruit(choice.pheromone);
			
		}else
		{
			//doStateExplore(Conditions(job->handle));
			if(choice.pheromone.positive) //Is a SOURCE Interface
			{
				if (!body->hasPacket)
				{
					if(sensor->senseBelow(choice.pheromone.inverse(),SENSOR_RADIUS_HUGE)>PHEROMONE_FLOOR)
					{
						job->transferFromJob(body, choice.pheromone.id);
						
					}
					doStateFindTrail(choice.pheromone.inverse(), choice.pheromone);
					

				}else
				{
					//doStateExplore(Conditions(job->handle))
					//throw("Should have packet here");
				}
			}else //Is a SINK Interface
			{

				if (sensor->senseBelow(choice.pheromone.inverse(), SENSOR_RADIUS_HUGE) > PHEROMONE_FLOOR)
				{
			
					
				}
				doStateFindTrail(choice.pheromone.inverse(), choice.pheromone);
			}
			
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

				//doStateFindTrail(jobReport.nextPartPheromone);
				doStateExplore(true,currentPheromone,true, jobReport.nextPartPheromone);
			}
			else
			{
				doStateExplore(true,currentPheromone);
				//report = TaskReport(FindJob, currentPheromone, jobReport.currentHandle);
				//taskDriver->setTask(new TaskPheromoneController());
			}
		}

	}*/
}

void TaskPheromoneController::doStateRecruit(PheromoneId recruitPheromone)
{
	state = states::Recruit;
	currentPheromone = { RECRUIT_PHEROMONE_ID,true };
	action->setAction(new ActionExplore(currentPheromone));
	//nextJobPheromone->setTrail(jobId, true);
	pheromone->setTrail(currentPheromone, PHEROMONE_TRAIL_START_STRENGTH*5, PHEROMONE_DECAY_VERY_FAST);
	pheromone->addTrail(recruitPheromone, PHEROMONE_TRAIL_START_STRENGTH*5, PHEROMONE_DECAY_VERY_FAST);
	declareStrenght = PHEROMONE_TRAIL_START_STRENGTH;

}

void TaskPheromoneController::stateRecruit()
{
	
	//pheromone->emit(currentPheromone, true, declareStrenght);
	auto pheromoneJob=pheromone->getTrail(currentPheromone);
	if(pheromoneJob.strenght< PHEROMONE_FLOOR)
	{
		
		doStateExplore();
	}
	
}


