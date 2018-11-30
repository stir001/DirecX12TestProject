#pragma once
#include "CollisionAction.h"
class PrimitiveController;

class TestAction :
	public CollisionAction
{
public:
	TestAction();
	~TestAction();
	void Action(int tag);
private:
	int mCounter;
};

