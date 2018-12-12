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

void TestAction2::OnAction(int tag1, int tag2)
{
}

void TestAction2::StayAction(int tag1, int tag2)
{
	if (tag1 == 2)
	{
		bool isHit = true;
	}
	if (tag2 == 0)
	{
		bool isTag2Hit = true;
	}
}

void TestAction2::ExitAction(int tag1, int tag2)
{
}
