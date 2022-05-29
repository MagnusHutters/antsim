#pragma once

#include <set>

#include "Job.h"

class JobMap
{
public:


	JobMap() {}

	bool areaIsOccupied(Vector2 pos, Vector2 size)
	{
		for (auto entity : entityList)
		{
			if (
				pos.x <= entity.second->pos.x && entity.second->pos.x < pos.x + size.x &&
				pos.y <= entity.second->pos.y && entity.second->pos.y < pos.y + size.y)
			{
				return true;
			}
		}
		return false;
	}


	void deleteEntity(int handle)
	{
		typename std::unordered_map<int, Job*>::iterator element = entityList.find(handle);
		if (element != entityList.end())
		{
			delete(element->second);
			entityList.erase(element);
			//delete(pointer);
		}
	}


	int registerEntity(Job* newEntity) {
		int handle = newId;
		//Entity* newEntity = new Entity(pos, handle);

		//throw("Test");


		entityList[handle] = newEntity;

		newId++;
		newEntity->handle = handle;

		return handle;
	}


	void setPosition(int handle, Vector2 pos) {
		entityList[handle]->pos = pos;
		throw("Test");

	}

	void updateAll()
	{
		for (std::pair<int, Job*> element : entityList)
		{
			element.second->update();

		}


	}

	const std::unordered_map<int, Job*>& getEntities() {
		return entityList;
	}

	std::set<int> getAllTypeIds()
	{
		std::set<int> typeIds;

		for (auto job : entityList)
		{
			auto interfaces = job.second->getInterfaces();
			for (const auto & interface : interfaces)
			{
				typeIds.insert(interface->typeId);
			}
		}
		return typeIds;
	}

	struct _getClosestResult { bool success; Job* entity; };
	_getClosestResult getClosest(const Vector2& point, const Conditions& conditions = Conditions()) {

		float closestDistSquare = (float)(9999 * 9999);
		int closestHandle = -1;
		bool success = false;

		for (std::pair<int, Job*> element : entityList)
		{
			if (!element.second->followsConditions(conditions))
			{
				continue;
			}

			float newDistSquare = getDistanceSquare(point, element.second->pos);
			if (newDistSquare < closestDistSquare) {
				closestDistSquare = newDistSquare;
				closestHandle = element.first;
				success |= true;
			}
		}
		if (success)
		{
			return { success, entityList[closestHandle] };
		}
		else
		{
			return { success,nullptr };
		}

	}


	float getDistanceSquare(Vector2 p1, Vector2 p2) {
		float difX = fabs(p2.x - p1.x);
		float difY = fabs(p2.y - p1.y);

		if (difX > MAP_WIDTH / 2) difX -= MAP_WIDTH;
		if (difY > MAP_HEIGHT / 2) difY -= MAP_HEIGHT;

		return (difX * difX) + (difY * difY);

	}
private:
	friend class Logger;

	std::unordered_map<int, Job*> entityList;

	int newId = 0;
};

