#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <list>

enum class BulletShapeType;

class CollisionAction : public btActionInterface
{
public:
	CollisionAction();
	virtual ~CollisionAction();

	virtual void Action(int tag) = 0;

	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	void debugDraw(btIDebugDraw* debugDrawer);

	void AddTargetTag(int target);

	void RemoveTargetTag(int target);

	const std::list<int>& GetTargetTags() const;

	void SetCollisionShape();
private:
	std::shared_ptr<btGhostObject> mGhost;
	std::shared_ptr<btCollisionShape>  mCollisionShape;

	std::list<int> mTargetTags;
};

