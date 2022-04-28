#pragma once

#include "Conditions.h"
#include "Vector2.h"


class Entity
{
public:
	int x;
	int y;
	int handle;
	Vector2 pos;

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
	Entity(Vector2 vec) : x(vec.x), y(vec.y), pos(vec) {}
	Entity(){}

	//inline int operator [](int i) const { return i ? y : x; }
	//inline int& operator [](int i) { return i ? y : x; }
};



