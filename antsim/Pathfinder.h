#pragma once
#include <queue>

#include "ObstacleMap.h"
#include <unordered_map>

class Pathfinder
{
public:

	struct PathCell
	{
		IntCoords intCoords = { -1,-1 };
		Vector2 realCoords = { -1,-1 };
		Vector2 direction = { 0,0 };
		int pathValue = -1;
	};
	Pathfinder()
	{
		exists = false;
	}

	struct _checkPathCell
	{
		bool success;
		PathCell cell;
	};
	_checkPathCell checkPathCell(IntCoords neigbour)
	{
		int x = neigbour.x;
		int y = neigbour.y;
		if(path.find(x)!=path.end())
		{
			if (path[x].find(y) != path[x].end())
			{
				return { true, path[x][y] };
			}
		}
		return { false, {} };
	}

	bool doPathfindStep(const Vector2& currentPos)
	{



		IntCoords currentCoord = pathQueue.front();


		
		int x = currentCoord.x;
		int y = currentCoord.y;
		pathQueue.pop();


		auto result = checkPathCell(currentCoord);
		if (result.success)
		{
			return false;
		}

		if (obstacles->isObstructed(currentCoord))
		{
			
			return false;
		}

		std::list<IntCoords> neigbours{ {x + 1,y},{x,y + 1} ,{x - 1,y} ,{x,y - 1} };

		PathCell newCell = { currentCoord,obstacles->getCellCenter(currentCoord),{0,0},0 };

		for (auto neigbour : neigbours)
		{
			auto result = checkPathCell(neigbour);
			if (!result.success) continue;

			PathCell checkCell = result.cell;
			if(checkCell.pathValue>= newCell.pathValue)
			{
				newCell.pathValue = checkCell.pathValue + 1;

				newCell.direction += (checkCell.realCoords - newCell.realCoords);

			}
		}
		newCell.direction.Normalize();

		Vector2 newCellCenter = obstacles->getCellCenter(newCell.intCoords);

		float distanceFromBody = newCellCenter.Distance(currentPos);
		if(distanceFromBody<closestCenter)
		{
			closestCenter = distanceFromBody;
		}

		auto checkCellIsEmpty = checkPathCell(currentCoord);
		if(!checkCellIsEmpty.success)
		{
			
			path[currentCoord.x][currentCoord.y] = newCell;
		}

		if(obstacles->checkLineOfSight(currentPos, newCellCenter))
		{
			currentWaypoint = obstacles->getCellCenter(newCell.intCoords);
			if(distanceFromBody<=closestCenter)
			{
				return true;
			}
		}


		for (auto neigbour : neigbours)
		{
			auto result = checkPathCell(neigbour);
			if(!result.success)
			{
				pathQueue.push(neigbour);
			}
			
		}
		return false;
	}

	bool doPathfindLoop(const Vector2& currentPos)
	{
		while(!pathQueue.empty())
		{
			
			
			if (doPathfindStep(currentPos)) return true;
			
		}
		return false;
	}

	Pathfinder(ObstacleMap* obstacles, const Vector2& target, const Vector2& currentPos)
		: obstacles(obstacles)
		  
	
	{
		reset(target, currentPos);
		exists = true;

	}

	void reset(const Vector2& target, const Vector2& currentPos)
	{
		
		this->target = target;
		targetIntCoords = obstacles->getMapCoords(target);

		pathQueue = std::queue<IntCoords>();
		for (auto value : path)
		{
			value.second.clear();
		}
		path.clear();

		pathQueue.push({ targetIntCoords });

		doPathfindLoop(currentPos);

	}

	
	Vector2 tryAdvance(Vector2 currentPos)
	{
		if (obstacles->checkLineOfSight(currentPos, target)) return target;

		if (!obstacles->checkLineOfSight(currentWaypoint, currentPos)) //If waypoint is obscurred. Calculate more map
		{
			//doPathfindLoop(currentPos);
			reset(target, currentPos);
			if (!obstacles->checkLineOfSight(currentWaypoint, currentPos))
			{
				reset(target, currentPos);
			}
		}else
		{
			

			for (int i = 1; i < 5; ++i)
			{
				IntCoords mapCoords = obstacles->getMapCoords(currentWaypoint);


				auto result = checkPathCell(mapCoords);
				if (!result.success)
				{
					reset(target,currentPos);
					continue;
				}

				Vector2 newWaypoint = currentWaypoint + path[mapCoords.x][mapCoords.y].direction;
				if (obstacles->checkLineOfSight(newWaypoint, currentPos))
				{
					currentWaypoint = newWaypoint;
				}
				else
				{
					i += 5;
				}
			}
			

		}

		return currentWaypoint;


	}
	bool exists = false;

	~Pathfinder()
	{
		for (auto value : path)
		{
			
			value.second.clear();
		}
		path.clear();
		
	}

private:
	ObstacleMap* obstacles;

	Vector2 currentWaypoint;

	Vector2 target;
	IntCoords targetIntCoords;
	Vector2 currentPos;

	float closestCenter = 99999999.0f;
	

	std::unordered_map<int, std::unordered_map<int, PathCell>> path;
	std::queue<IntCoords> pathQueue;
	
};

