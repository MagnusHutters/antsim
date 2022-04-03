#pragma once

#include "Vector2.h"
#include "Body.h"
#include "SensorDriver.h"


enum actions {hold, followTrail, explore};

class Action
{
public:

	

	Action() : body(nullptr), sensor(nullptr) {}

	virtual void init() {

	}

	virtual void calcAction() {
		body->breakMotion();
	}
	virtual void doAction() {}
private:

	friend class ActionDriver;
	friend class ActionFollowTrail;
	friend class ActionHold;
	friend class ActionExplore;

	SensorDriver* sensor;
	BodyDriver* body;
	void transferDrivers(SensorDriver* sensor, BodyDriver* body);


};


class ActionDriver
{
public:
	ActionDriver();
	ActionDriver(SensorDriver* sensor, BodyDriver* body);



	void setAction(Action* newAction); 

	void calcAction();
	void doAction();


private:
	SensorDriver* sensor;
	BodyDriver* body;


	Action* nextAction;

	Action* action;
	bool isNewAction = false;

};

