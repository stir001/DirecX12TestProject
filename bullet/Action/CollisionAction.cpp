#include "CollisionAction.h"
#include "bullet/System/PhysicsSystem.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/Ghost/BulletGhostObject.h"
#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

CollisionAction::CollisionAction(std::shared_ptr<BulletGhostObject> collision): mCollision(collision)
{
}


CollisionAction::~CollisionAction()
{
}

void CollisionAction::updateAction(btCollisionWorld * collisionWorld, btScalar deltaTimeStep)
{
	int num = mCollision->GetGhostObject()->getNumOverlappingObjects();
	for (int i = 0; i < num; ++i)
	{
		int tag = mCollision->GetGhostObject()->getOverlappingObject(i)->getIslandTag();
		auto fitr =std::find_if(mTargetTags.begin(), mTargetTags.end(), [tag](int value) {return tag == value; });
		if (fitr != mTargetTags.end())
		{
			Action(*fitr);
		}
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

void CollisionAction::SetCollisionObject(std::shared_ptr<BulletGhostObject> shape)
{
	mCollision = shape;
}
