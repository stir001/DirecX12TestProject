#include "TestAction.h"
#include "bullet/Shape/BoxCollisionShape.h"
#include "bullet/System/PhysicsSystem.h"
#include "bullet/Ghost/BulletGhostObject.h"
#include "Dx12MSLib.h"

TestAction::TestAction(): mCounter(0)
{
	auto shape = std::make_shared<BoxCollisionShape>(DirectX::XMFLOAT3(1.f,1.f,1.f));
	SetShape(shape);
}


TestAction::~TestAction()
{
}

void TestAction::Action(int tag)
{
	mGhost->GetGhostObject()->setCustomDebugColor(btVector3(1, 0, 1));
}


