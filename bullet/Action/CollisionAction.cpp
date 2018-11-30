#include "CollisionAction.h"

#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/Ghost/BulletGhostObject.h"

#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

CollisionAction::CollisionAction()
	: mCollision(std::make_shared<BulletGhostObject>())
{
	auto trans = mCollision->GetGhostObject()->getWorldTransform();
	trans.setOrigin(btVector3(0, 0, 10));
	mCollision->GetGhostObject()->setWorldTransform(trans);
	int num = mCollision->GetGhostObject()->getCollisionFlags();
	//mCollision->GetGhostObject()->setActivationState(DISABLE_DEACTIVATION);
	//mCollision->GetGhostObject()->setCollisionFlags(num | btCollisionObject::CollisionFlags::CF);
}


CollisionAction::~CollisionAction()
{
}

void CollisionAction::updateAction(btCollisionWorld * collisionWorld, btScalar deltaTimeStep)
{
	int dnum = collisionWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < dnum; ++i)
	{
		auto manifold = collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
		auto objA = manifold->getBody0();
		auto objB = manifold->getBody1();

		auto tag0 = objA->getUserIndex();
		auto tag1 = objB->getUserIndex();
		
		if (manifold->getNumContacts() > 0)
		{
			int a = 0;
		}
	}

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

void CollisionAction::debugDraw(btIDebugDraw* debugDrawer)
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

void CollisionAction::SetShape(std::shared_ptr<BulletCollisionShape> shape)
{
	mCollision->SetCollisionShape(shape);
}

void CollisionAction::SetCollisionState(int state)
{
	mCollision->GetGhostObject()->setCollisionFlags(state);

}

void CollisionAction::Translate(float x, float y, float z)
{
	auto trans = mCollision->GetGhostObject()->getWorldTransform();
	trans.setOrigin({ x,y,z });
	mCollision->GetGhostObject()->setWorldTransform(trans);
}
