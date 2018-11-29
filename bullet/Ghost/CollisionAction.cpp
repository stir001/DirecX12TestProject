#include "CollisionAction.h"
#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include "bullet/System/PhysicsSystem.h"

CollisionAction::CollisionAction()
{
}


CollisionAction::~CollisionAction()
{
}

void CollisionAction::updateAction(btCollisionWorld * collisionWorld, btScalar deltaTimeStep)
{
	int num = mGhost->getNumOverlappingObjects();
	for (int i = 0; i < num; ++i)
	{
		int tag = mGhost->getOverlappingObject(i)->getIslandTag();
	}
}

void CollisionAction::debugDraw(btIDebugDraw * debugDrawer)
{
}

void CollisionAction::AddTargetTag(int target)
{
	auto itr = std::find_if(mTargetTags.begin(), mTargetTags.end(), [target](int value) {return value == target; });
	if (itr == mTargetTags.end())
	{
		mTargetTags.push_back(target);
	}
}

void CollisionAction::RemoveTargetTag(int target)
{
	auto itr = std::find_if(mTargetTags.begin(), mTargetTags.end(), [target](int value) {return value == target; });
	if (itr == mTargetTags.end())
	{
		mTargetTags.erase(itr);
	}
}

const std::list<int>& CollisionAction::GetTargetTags() const
{
	return mTargetTags;
}

void CollisionAction::SetCollisionShape()
{
	mGhost = std::make_shared<btGhostObject>();
}
