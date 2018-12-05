#include "stdafx.h"
#include "TestAction2.h"
#include "Bullet/System/PhysicsSystem.h"

TestAction2::TestAction2(std::shared_ptr<BulletCollisionShape> shape, int tag)
	:IActionDefiner(shape,tag)
{
}


TestAction2::~TestAction2()
{
}

void TestAction2::Action(int tag)
{
	if (tag != -1)
	{
		int a = 0;
	}
}