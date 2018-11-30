#include "BulletGhostObject.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


BulletGhostObject::BulletGhostObject()
{
	mGhost = std::make_shared<btGhostObject>();
	mGhost->setUserIndex(3);
}

BulletGhostObject::~BulletGhostObject()
{
}

std::shared_ptr<btGhostObject> BulletGhostObject::GetGhostObject()
{
	return mGhost;
}

void BulletGhostObject::SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape)
{
	mShape = collisionShape;
	mGhost->setCollisionShape(mShape->GetShape().get());
}
