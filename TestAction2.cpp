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

void TestAction2::OnAction(int tag)
{
}

void TestAction2::StayAction(int tag)
{
	if (tag == 2)
	{
		bool isHit = true;
	}
}

void TestAction2::ExitAction(int tag)
{
}
