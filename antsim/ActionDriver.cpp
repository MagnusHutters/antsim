#include "ActionDriver.h"

ActionDriver::ActionDriver()
{
	action = new Action();
}

ActionDriver::ActionDriver(SensorDriver* sensor, BodyDriver* body) : sensor(sensor), body(body)
{
	action = new Action();
	action->body = body;
	action->sensor = sensor;
}

void ActionDriver::setAction(Action* newAction)
{
	nextAction = newAction;
	isNewAction = true;
}

void ActionDriver::calcAction()
{
	action->calcAction();
}

void ActionDriver::doAction()
{
	action->doAction();
	if (isNewAction) {
		isNewAction = false;

		nextAction->sensor = action->sensor;
		nextAction->body = action->body;
		

		delete(action);
		action = nextAction;

		action->init();
	}
}

void Action::transferDrivers(SensorDriver* sensor, BodyDriver* body) 
{
	this->sensor = sensor;
	this->body = body;
}
