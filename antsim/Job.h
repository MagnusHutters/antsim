#pragma once




#include "Config.h"
#include "Vector2.h"
//#include "EntityMap.h"
#include "Body.h"
#include "Entity.h"



class JobInterface
{
public:
	JobInterface(int typeId, bool positive, float maxAmount= JOB_MAX_AMOUNT)
		: positive(positive),
		  typeId(typeId),
		  max_amount(maxAmount),
		  amount(positive ? maxAmount : 0)
	{
	}


	bool positive; //Positive(true): source, Negative(false): sink
	int typeId;
	float amount;
	const float max_amount;

	int getPriority() const
	{
		float percentage = amount / max_amount;
		if (!positive) percentage = 1 - percentage;
		return (sqrtf(percentage)* WEIGHT_SIZE);
	}

	inline bool hasCapacity(float hasCapacityAmount= PACKET_SIZE) const
	{
		return (amount + hasCapacityAmount) <= max_amount;
	}
	inline bool hasAmount(float hasAmount= PACKET_SIZE) const
	{
		return amount >= hasAmount;
	}

	bool doesMatchPheromone(PheromoneId pheromone) {
		return (pheromone.id == typeId && pheromone.positive == positive);
	}

	PheromoneId getPheromone()
	{
		return { typeId,positive };
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
	

	//Job();
	
	Job(Vector2 pos, int typeId, bool positive) : interfaces({new JobInterface(typeId, positive)}), Entity(pos) {

		//handle = jobMap->registerEntity(this, pos);
	}
	
	
	struct _GetInterfaceResult { bool sucess; JobInterface* interface; };
	_GetInterfaceResult getInterface(PheromoneId pheromone) const
	{
		for (const auto interface : interfaces)
		{
			if(interface->doesMatchPheromone(pheromone))
			{
				return { true,interface };
			}
		}
		return { false,nullptr };
	}

	//struct weightedPheromone { float weight; exploredPheromone pheromone; };
	struct _getAllJobInterfacesResult { std::vector<int> weights; std::vector<PheromoneId> pheromones; };
	_getAllJobInterfacesResult getAllJobInterfaceWeights()
	{
		_getAllJobInterfacesResult result;
		for (auto interface : interfaces)
		{
			
			result.weights.emplace_back(interface->getPriority());
			result.pheromones.emplace_back(interface->getPheromone());

		}
		return result;
	}


	
	bool transferToJob(BodyDriver* body)
	{
		if(body->hasPacket)
		{
			auto result = getInterface({ body->packet->getTypeId(),false });
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
		auto result = getInterface({typeId,true});
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
		bool anyPheromoneMatch = false;
		for (const auto pheromone : conditions.pheromoneWhitelist)
		{
			for (auto interface : interfaces)
			{
				anyPheromoneMatch |= interface->doesMatchPheromone(pheromone);
			}
		}
		if(conditions.pheromoneWhitelist.size()==0)
		{
			anyPheromoneMatch = true;
		}

		bool anyJobMatch = false;
		for (int jobCondition : conditions.jobBlacklist)
		{
			anyJobMatch |= jobCondition == handle;
		}

		return anyPheromoneMatch && !anyJobMatch; //anyPheromoneMatch must be true and anyJobMatch must be false
		
	}

	

	

	//virtual bool getReady() {
	//	return false;
	//}
	//int pheromoeId;
protected:



	/*
	virtual JobReport interact(BodyDriver* body)
	{
		if (!canInteract()) return JobReport(this, false);

		if(haveNextJob)
		{
			return JobReport(this);
		}
		return JobReport(this);


	}*/

	std::list<Process*> processes;
	std::list<JobInterface*> interfaces;
	
	//int nextJobHandle = -1;
	//bool haveNextJob = false;
	//Vector2 pos;
	//EntityMap<Job>* jobMap;
	//int handle;
	//int nextJobPheromone=-1;
	


	//float resourcePool = 1;
	//float maxResource = 10;
	//float resourceGeneration = 0.1;
};

