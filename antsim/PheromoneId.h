#pragma once
class PheromoneId
{

public:
	int id;
	bool positive;


	bool operator==(const PheromoneId& pheromone) const
	{
		return id == pheromone.id && positive == pheromone.positive;
	}
};

