#pragma once




#include "Config.h"
#include "Vector2.h"
//#include "EntityMap.h"
#include "Body.h"
#include "Entity.h"



class JobInterface
{
public:
	JobInterface(bool positive, int typeId, float maxAmount= JOB_MAX_AMOUNT)
		: positive(positive),
		  typeId(typeId),
		  max_amount(maxAmount)
	{
	}


	bool positive; //Positive(true): source, Negative(false): sink
	int typeId;
	float amount;
	const float max_amount;

	float getPriority() const
	{
		float percentage = amount / max_amount;
		if (!positive) percentage = 1 - percentage;
		return sqrtf(percentage);
	}

	inline bool hasCapacity(float hasCapacityAmount= PACKET_SIZE) const
	{
		return (amount + hasCapacityAmount) <= max_amount;
	}
	inline bool hasAmount(float hasAmount= PACKET_SIZE) const
	{
		return amount >= hasAmount;
	}
	


};

class Process
{
public:
	Process(JobInterface* input1, JobInterface* input2, JobInterface* output)
		: output({output,1}),
		  inputs({{input1,1},{input2,1}})
	{
	}
	int process(int number) const
	{
		int count;
		for (count = 0; count < number; ++count)
		{
			if (!process()) break;
		}
		return count;
	}
	int process() const
	{
		bool haveSources = true;
		for (auto input : inputs)
		{
			if (input.first->amount < input.second) haveSources = false;
		}
		if(haveSources)
		{
			for (auto input : inputs)
			{
				input.first->amount -= input.second;
			}
			output.first->amount += output.second;
			return 1;
		}
		return 0;
	}

private:

	
	std::pair<JobInterface*, float> output;
	std::list<std::pair<JobInterface*, float>> inputs; //Input : Multiplier
};


class Job : public Entity
{
public:
	struct JobReport {

		bool successful;
		bool isNextPart;
		int currentPheromone;
		int currentHandle;
		int nextPartHandle;
		int nextPartPheromone;
		JobReport() :
			successful(false),
			isNextPart(false),
			currentPheromone(-1),
			nextPartHandle(-1)
		{}

		JobReport(Job* job) : //Basic info for non interaction
			successful(true),
			isNextPart(job->haveNextJob),
			currentPheromone(job->pheromoeId),
			currentHandle(job->handle),
			nextPartHandle(job->nextJobHandle),
			nextPartPheromone((job->nextJobPheromone))

		{}
		JobReport(Job* job, bool success) : //Basic info for non interaction
			successful(success),
			isNextPart(job->haveNextJob),
			currentPheromone(job->pheromoeId),
			currentHandle(job->handle),
			nextPartHandle(job->nextJobHandle),
			nextPartPheromone((job->nextJobPheromone))

		{}

		
	};

	//Job();
	Job(Vector2 pos, int id) : pheromoeId(id), Entity(pos) {
		
		//handle = jobMap->registerEntity(this, pos);
	}
	Job(Vector2 pos, int id) : pheromoeId(id), Entity(pos) {

		//handle = jobMap->registerEntity(this, pos);
	}


	void setNextJob(int handle, int pheromone)
	{
		nextJobHandle = handle;
		nextJobPheromone = pheromone;
		haveNextJob = true;
	}

	const JobReport& doInteract(BodyDriver* body){
		if(body->body.pos.Distance(pos)>JOB_INTERACT_RADIUS) { // Out of range
			return JobReport();
		}
			//return JobReport(this);
		if(canInteract())
		{
			return interact(body);
		}else
		{
			return JobReport(this, false);
		}
		
	}
	struct _GetInterfaceResult { bool sucess; JobInterface* interface; };
	_GetInterfaceResult getInterface(int typeId) const
	{
		for (const auto interface : interfaces)
		{
			if(interface->typeId==typeId)
			{
				return { true,interface };
			}
		}
		return { false,nullptr };
	}


	
	bool transferToJob(BodyDriver* body)
	{
		if(body->hasPacket)
		{
			auto result = getInterface(body->packet->getTypeId());
			if(result.sucess)
			{
				if(result.interface->hasCapacity())
				{
					result.interface->amount += PACKET_SIZE;
					delete(body->packet);
					body->hasPacket = false;
					return true;
				}
				else
				{
					return false;
				}
			}else
			{
				throw("Interface did not exist");
				return false;
			}
			
		}else
		{
			throw("Trying to transfer when empty");
			return false;
		}
	}

	bool transferFromJob(BodyDriver* body, int typeId)
	{
		auto result = getInterface(typeId);
		if(result.sucess)
		{
			if(result.interface->hasAmount())
			{
				body->packet = new Packet(typeId);
				body->hasPacket = true;
				result.interface->amount -= PACKET_SIZE;

				return true;
			}else
			{
				return false;
			}
		}else
		{
			throw("Interface did not exist");
			return false;
		}
	}

	const JobReport& doGetInfo(BodyDriver* body) {
		if (body->body.pos.Distance(pos) > JOB_INTERACT_RADIUS) { // Out of range
			return JobReport();
		}
		if (!haveNextJob) {
			return JobReport(this);
		}

	}

	inline virtual bool canInteract()
	{
		return true;// return resourcePool > 1;
	}

	void update()
	{
		for (auto process : processes)
		{
			process->process();
		}
	}

	bool followsConditions(const Conditions& conditions)
	{
		bool anyMatch = false;
		for (int conditon : conditions.list)
		{
			if (conditon == pheromoeId) anyMatch = true;
		}
		if(conditions.conditionType==Blacklist)
		{
			return !anyMatch;
		}else
		{
			return anyMatch;
		}
	}

	

	

	//virtual bool getReady() {
	//	return false;
	//}
	int pheromoeId;
protected:



	virtual JobReport interact(BodyDriver* body)
	{
		if (!canInteract()) return JobReport(this, false);

		if(haveNextJob)
		{
			return JobReport(this);
		}
		return JobReport(this);


	}

	std::list<Process*> processes;
	std::list<JobInterface*> interfaces;
	
	int nextJobHandle = -1;
	bool haveNextJob = false;
	//Vector2 pos;
	//EntityMap<Job>* jobMap;
	//int handle;
	int nextJobPheromone=-1;
	


	//float resourcePool = 1;
	//float maxResource = 10;
	//float resourceGeneration = 0.1;
};

