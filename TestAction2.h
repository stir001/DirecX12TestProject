#pragma once
#include "Bullet/Action/IActionDefiner.h"

class BulletCollisionShape;

class TestAction2 :
	public IActionDefiner
{
public:
	TestAction2(std::shared_ptr<BulletCollisionShape> shape, int tag);
	~TestAction2();

	void Action(int tag);
};

