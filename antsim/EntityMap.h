#pragma once




#include "Config.h"
#include "Vector2.h"


#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>













template <typename T>
class EntityMap
{
public:

	struct Entity {
		int x;
		int y;
		Vector2 pos;
		T* object;
		//Entity(T* objectPointer, int x, int y) :		object(objectPointer), x(x), y(y) {}
		//Entity(T* objectPointer, float x, float y) : object(objectPointer), x(x), y(y), pos(Vector2(x,y)) {}
		Entity(T* objectPointer, Vector2 vec) : object(objectPointer), x(vec.x), y(vec.y), pos(vec) {}

		inline int operator [](int i) const { return i ? y : x; }
		inline int& operator [](int i) { return i ? y : x; }

	};

	EntityMap() {}




	std::unordered_map<int, Entity*> entityList;




	int registerEntity(T* object, Vector2 pos) {

		Entity* newEntity = new Entity(object, pos);


		int handle = newId;
		entityList[handle] = newEntity;

		newId++;

		return handle;
	}

	void setPosition(int handle, Vector2 pos) {
		entityList[handle]->pos = pos;

	}

	const std::unordered_map<int, Entity*>& getEntities() {
		return entityList;
	}

	const Entity& getClosest(Vector2 point) {

		float closestDistSquare = MAP_CELLS* MAP_CELLS;
		int closestHandle = -1;
		for (std::pair<int, Entity*> element : entityList)
		{
			float newDistSquare = getDistanceSquare(point, element.second->pos);
			if (newDistSquare < closestDistSquare) {
				closestDistSquare = newDistSquare;
				closestHandle = element.first;
			}
		}

	}

	inline float getDistanceSquare(Vector2 p1, Vector2 p2) {
		float difX = fabs(p2.x - p1.x);
		float difY = fabs(p2.y - p1.y);
		
		if (difX > MAP_WIDTH / 2) difX -= MAP_WIDTH;
		if (difY > MAP_HEIGHT / 2) difY -= MAP_HEIGHT;

		return (difX * difX) + (difY * difY);

	}


	int newId = 0;
};






/*

template <typename T>
class EntityMap
{
public:

	EntityMap(int cellSize) : cellSize(cellSize) {
		width = MAP_WIDTH / cellSize;
		height = MAP_HEIGHT / cellSize;
		cellSizeMultiplier = 1.0 / cellSize;
	}
	int registerEntity(T* object, Vector2 pos) {

		Entity* newEntity = new Entity(object, pos);

		int handle = newId;
		entityCell[handle] =newEntity;

		newId++;

		return handle;
	}


	void setPosition(int handle, Vector2 pos) {
		pos *= cellSizeMultiplier;
		Entity* entity = entityCell[handle];

		oldX0 = entity.x;
		oldY0 = entity.y;
		oldX1 = wrapWidth(oldX+1);
		oldY1 = wrapHeight(oldY+1);
		newX0 = pos.x;
		newY0 = pos.y;
		newX1 = wrapWidth(newX0 +1);
		newY1 = wrapHeight(newY1 +1);
		//difX = wrapWidth(newX - oldX);
		//difY = wrapHeight(newY - oldY);

		

		

		


		entiry.pos = pos;

	}

private:


	int wrapWidth(int val);

	int wrapHeight(int val);

	void removeEntityFromMap(int x, int y, int handle);
	void addEntityToMap(int x, int y, int handle);


	struct Entity {
		int x;
		int y;
		Vector2 pos;
		T* object;
		//Entity(T* objectPointer, int x, int y) :		object(objectPointer), x(x), y(y) {}
		//Entity(T* objectPointer, float x, float y) : object(objectPointer), x(x), y(y), pos(Vector2(x,y)) {}
		Entity(T* objectPointer, Vector2 vec) :		object(objectPointer), x(vec.x), y(vec.y), pos(vec) {}

		inline int operator [](int i) const { return i ? y : x; }
		inline int& operator [](int i) { return i ? y : x; }

	};


	int cellSize, width, height;
	float cellSizeMultiplier;

	std::vector< std::vector<std::unordered_set<int>>> entityMap;

	std::unordered_map<int, Entity*> entityCell;

	int newId = 0;
};

*/
