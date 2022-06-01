#pragma once




#include "Config.h"
#include "Vector2.h"
//#include "EntityMap.h"
#include "Body.h"
#include "Entity.h"



class JobInterface
{
public:
	JobInterface(int typeId, bool positive, float amount)
		: positive(positive),
		  typeId(typeId),
		  max_amount(JOB_MAX_AMOUNT),
		  amount(amount)
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
		return static_cast<int>(sqrtf(percentage) * WEIGHT_SIZE);
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

	Process(JobInterface* input1, JobInterface* output)
		: output({ output,1 }),
		inputs({ {input1,1} })
	{
	}

	Process(JobInterface* terminus, float amount=1.0f)
		: output({ terminus,terminus->positive ? amount : 0 }),
		inputs({ {terminus,terminus->positive ? 0 : amount} })
	{
	}
	int process(int number)
	{
		int count;
		for (count = 0; count < number; ++count)
		{
			if (!process()) break;
		}
		return count;
	}
	int process()
	{
		bool haveSources = true;
		for (auto input : inputs)
		{
			if (input.first->amount < input.second) haveSources = false;
		}
		if(haveSources)
		{
			if(output.first->amount < output.first->max_amount - output.second)
			{
				for (auto input : inputs)
				{
					input.first->amount -= input.second;
				}
				output.first->amount += output.second;
				totalProducts+=1;
				return 1;
			}
			
		}
		return 0;
	}

	int getTotal()
	{
		return totalProducts;
	}

private:

	int totalProducts = 0;
	friend class Logger;
	
	std::pair<JobInterface*, float> output;
	std::list<std::pair<JobInterface*, float>> inputs; //Input : Multiplier
};


class Job : public Entity
{
public:
	

	//Job();
	
	Job(Vector2 pos) : Entity(pos) {

		//handle = jobMap->registerEntity(this, pos);
	}

	static Job* createSource(Vector2 pos, int typeId, bool endless=false, float amount=1.0f, int startAmunt= JOB_MAX_AMOUNT)
	{
		Job* job = new Job(pos);
		//int variance = rand() % 10;
		JobInterface* newInterface=job->addOutputInterface(typeId, static_cast<float>(startAmunt));
		if(endless)
		{
			job->processes.push_back(new Process(newInterface, amount));
		}
		return job;
	}

	static Job* createSink(Vector2 pos, int typeId, bool endless = false)
	{
		Job* job = new Job(pos);
		JobInterface* newInterface = job->addInputInterface(typeId);
		if (endless)
		{
			job->processes.push_back(new Process(newInterface));
		}
		return job;
	}

	static Job* createIntermediary(Vector2 pos, int input1, int output)
	{
		Job* job = new Job(pos);
		
		JobInterface* input1Interface = job->addInputInterface(input1,0);
		JobInterface* outputInterface = job->addOutputInterface(output,0);

		job->processes.push_back(new Process(input1Interface, outputInterface));
		
		return job;
	}
	static Job* createIntermediary(Vector2 pos, int input1, int input2, int output)
	{
		Job* job = new Job(pos);

		JobInterface* input1Interface = job->addInputInterface(input1);
		JobInterface* input2Interface = job->addInputInterface(input2);
		JobInterface* outputInterface = job->addOutputInterface(output,0);

		job->processes.push_back(new Process(input1Interface, input2Interface, outputInterface));

		return job;
	}


	JobInterface* addOutputInterface(int typeId, float amount = JOB_MAX_AMOUNT)
	{
		auto* job = new JobInterface(typeId, true, amount);
		interfaces.push_back(job);
		return job;
	}
	JobInterface* addInputInterface(int typeId, float amount = 0)
	{
		auto job = new JobInterface(typeId, false, amount);
		interfaces.push_back(job);
		return job;
	}
	
	std::list<float> getAmounts()
	{
		std::list<float> list;
		for (auto interface : interfaces)
		{
			list.push_back(interface->amount);
		}
		return list;
	}

	std::list<JobInterface*> getInterfaces()
	{
		return interfaces;
	}

	void haveInteracted()
	{
		interactions++;
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

	JobInterface* getOutput()
	{
		for (auto interface : interfaces)
		{
			if (interface->positive)
			{
				return interface;
			}

		}
		return nullptr;
	}

	float getOutputLevel() {
		for (auto interface : interfaces)
		{
			if(interface->positive)
			{
				return interface->amount;
			}
		
		}
		return 0;
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

					body->endPath();
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

				body->startPath(handle);

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
			process->process(1);
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

	friend class Logger;

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
	int interactions = 0;
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

