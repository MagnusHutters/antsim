#pragma once
#include <array>
#include <unordered_map>

#include "PheromoneUtility.h"
#include "Vector2.h"

class RigidBodyMap
{
public:
	
	struct RigidBodyInfo { Vector2 pos; IntCoords index={-1,-1}; int handle=-1; };

	RigidBodyMap()
	{

		xMaxIndex = MAP_WIDTH / RIGID_BODY_CELL_SIZE;
		yMaxIndex = MAP_HEIGHT / RIGID_BODY_CELL_SIZE;
	}


	//SPECIFIC

	Vector2 checkMovement(int handle, Vector2 oldPos, Vector2 newPos)
	{
		Vector2 movement = newPos - oldPos;
		float squareRange = (RIGID_BODY_RADIUS * 2) * (RIGID_BODY_RADIUS * 2);

		std::list<RigidBodyInfo> bodiesInRange;
		getBodiesInRange(newPos, RIGID_BODY_RADIUS * 4, bodiesInRange);


		std::list<RigidBodyInfo> collisions;

		for (auto bodyInfo : bodiesInRange)
		{
			if (bodyInfo.handle == handle) continue;
			float squareDistToAttempt = newPos.DistanceSquared(bodyInfo.pos);
			if(squareDistToAttempt <= squareRange)
			{
				collisions.push_back(bodyInfo);
			}
		}

		Vector2 collisionModifier{0,0};
		for (RigidBodyInfo collision : collisions)
		{
			float dist = collision.pos.Distance(newPos);
			float overlap = (RIGID_BODY_RADIUS * 2) - dist;
			Vector2 normal = (newPos - collision.pos);
			normal.Normalize();
			normal *= overlap;
			collisionModifier += normal;
		}
		return newPos + collisionModifier;


		/*for (int i = 0; i < RIGID_BODY_MOVEMENT_ITERATIONS; ++i)
		{
			Vector2 newAtemptPos = oldPos + movement;
			bool isIntersecting = false;

			for (auto bodyInfo : bodiesInRange)
			{
				if (bodyInfo.handle == handle) continue;
				float squareDistToAttempt = newAtemptPos.DistanceSquared(bodyInfo.pos);
				isIntersecting |= (squareDistToAttempt <= squareRange);
			}
			if(!isIntersecting)
			{
				return newAtemptPos;
			}
			movement *= 0.5;
		}*/
		return oldPos;
	}



	//===============SEARCH=======================

	std::list<RigidBodyInfo> getBodiesInRange(Vector2 pos, float range)
	{
		std::list<RigidBodyInfo> bodiesInRange;
		getBodiesInRange(pos, range, bodiesInRange);
		return bodiesInRange;
	}

	std::list<RigidBodyInfo> getBodiesInRange(Vector2 pos, float range, std::list<RigidBodyInfo>& returnList)
	{
		float squareRange = range* range;
		auto index = getIndexFromPos(pos);
		int indexSearchRange = static_cast<int>(range / RIGID_BODY_CELL_SIZE)+1;
		IntCoords searchIndex = index;
		


		for (int x = index.x- indexSearchRange; x <= index.x+indexSearchRange; ++x)
		{
			for (int y = index.y - indexSearchRange; y <= index.y + indexSearchRange; ++y)
			{
				searchIndex = {x,y};
				getInsideBounds(searchIndex);

				for (auto value : map[searchIndex.x][searchIndex.y])
				{
					float squareDist = value.second.pos.DistanceSquared(pos);
					if(squareDist<=squareRange)
					{
						returnList.push_back(value.second);

					}
				}
			}

		}

		return returnList;

	}




	//===============MODIFIERS====================
	int deleteEntity(int handle)
	{
		IntCoords index = currentMapPositions[handle];
		map[index.x][index.y].erase(handle);
		currentMapPositions.erase(handle);

	}
	void move(int handle, Vector2 newPos)
	{
		auto newIndex = getIndexFromPos(newPos);
		transferIndex(handle, newIndex);

		map[newIndex.x][newIndex.y][handle] = { newPos,newIndex,handle };
	}
	int registerEntity(Vector2 pos)
	{
		IntCoords index = getIndexFromPos(pos);
		int handle = handleCounter++;

		currentMapPositions[handle] = index;
		map[index.x][index.y][handle] = {pos,index,handle};

		return handle;
	}
	


private:
	friend class Logger;

	IntCoords& getInsideBounds(IntCoords &coords)
	{
		if (coords.x < 0) coords.x += xMaxIndex;
		else if (coords.x >= xMaxIndex) coords.x -= xMaxIndex;

		if (coords.y < 0) coords.y += yMaxIndex;
		else if (coords.y >= yMaxIndex) coords.y -= yMaxIndex;

		return coords;

	}

	void transferIndex(int handle, IntCoords& newIndex)
	{
		IntCoords currentIndex = currentMapPositions[handle];
		if (newIndex == currentIndex) return;
		getInsideBounds(newIndex);
		map[newIndex.x][newIndex.y][handle] = map[currentIndex.x][currentIndex.y][handle];


		map[currentIndex.x][currentIndex.y].erase(handle);

		currentMapPositions[handle] = newIndex;
	}

	

	IntCoords getIndexFromPos(Vector2 pos)
	{
		return { (int)(pos.x * (1.0 / RIGID_BODY_CELL_SIZE)), (int)(pos.y * (1.0 / RIGID_BODY_CELL_SIZE)) };
	}

	std::array<std::array<std::unordered_map<int, RigidBodyInfo>, MAP_HEIGHT / RIGID_BODY_CELL_SIZE>, MAP_WIDTH / RIGID_BODY_CELL_SIZE> map;
	std::unordered_map<int, IntCoords> currentMapPositions;

	int xMaxIndex;
	int yMaxIndex;

	int handleCounter = 0;



};

