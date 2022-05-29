#pragma once

#include "Conditions.h"
#include "Vector2.h"


class Entity
{
public:
	int x=-1;
	int y=-1;
	int handle=-1;
	Vector2 pos=Vector2();

	virtual void update()
	{
		
	}

	virtual bool followsConditions(const Conditions& conditions)
	{
		return true;
	}
	//T* object;
	//Entity(T* objectPointer, int x, int y) :		object(objectPointer), x(x), y(y) {}
	//Entity(T* objectPointer, float x, float y) : object(objectPointer), x(x), y(y), pos(Vector2(x,y)) {}
	Entity(Vector2 vec) : x(static_cast<int>(vec.x)), y(static_cast<int>(vec.y)), pos(vec), handle(-1) {}
	Entity(){}

	//inline int operator [](int i) const { return i ? y : x; }
	//inline int& operator [](int i) { return i ? y : x; }
};



