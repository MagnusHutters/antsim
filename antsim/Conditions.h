#pragma once

#include <list>

enum blacklistWHitelist { Blacklist, Whitelist };

class Conditions
{
public:
	Conditions(const std::list<int>& list, blacklistWHitelist conditionType)
		: list(list),
		  conditionType(conditionType)
	{
	}

	Conditions(int condition, blacklistWHitelist conditionType)
		: list({ condition }),
		conditionType(conditionType)
	{
	}
	Conditions()
	{
		conditionType = Blacklist;
		list = {};
	}
	


	
	
	std::list<int> list;
	blacklistWHitelist conditionType; //

};

