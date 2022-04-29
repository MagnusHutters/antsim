#pragma once

#include <list>
#include <utility>

#include "PheromoneId.h"


class Conditions
{
public:
	Conditions(std::list<PheromoneId> list)
		: pheromoneWhitelist(std::move(list)), jobBlacklist({})
	{
	}
	
	
	Conditions(PheromoneId pheromoneId)
		: pheromoneWhitelist({ pheromoneId }), jobBlacklist({})

	{
	}
	Conditions(int jobId)
		: pheromoneWhitelist({  }), jobBlacklist({ jobId })

	{
	}
	Conditions(std::list<int> jobBlacklist)
		: pheromoneWhitelist({  }), jobBlacklist(std::move(jobBlacklist))

	{
	}

	Conditions() : pheromoneWhitelist({}), jobBlacklist({})
	{
	}
	
	std::list<PheromoneId> pheromoneWhitelist;
	std::list<int> jobBlacklist;
	//blacklistWHitelist conditionType; //

	Conditions operator+(const Conditions& conditions) const
	{
		Conditions newCon = *this;
		newCon.pheromoneWhitelist.insert(newCon.pheromoneWhitelist.end(), conditions.pheromoneWhitelist.begin(), conditions.pheromoneWhitelist.end());
		newCon.jobBlacklist.insert(newCon.jobBlacklist.end(), conditions.jobBlacklist.begin(), conditions.jobBlacklist.end());
		return newCon;
	}
};

