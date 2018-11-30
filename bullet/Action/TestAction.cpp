#include "TestAction.h"
#include "bullet/Shape/BoxCollisionShape.h"
#include "bullet/System/PhysicsSystem.h"
#include "Dx12MSLib.h"

TestAction::TestAction(): mCounter(0)
{
	auto shape = std::make_shared<BoxCollisionShape>(DirectX::XMFLOAT3(1.f,1.f,1.f));
	SetShape(shape);
	PhysicsSystem::Instance().AddGhost(mCollision);
}


TestAction::~TestAction()
{
}

void TestAction::Action(int tag)
{
}


