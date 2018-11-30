#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <list>

enum class BulletShapeType;
class BulletGhostObject;

class CollisionAction : public btActionInterface
{
public:
	CollisionAction(std::shared_ptr<BulletGhostObject> shape);
	virtual ~CollisionAction();

	virtual void Action(int tag) = 0;

	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	void debugDraw(btIDebugDraw* debugDrawer);

	void AddTargetTag(int target);

	void RemoveTargetTag(int target);

	const std::list<int>& GetTargetTags() const;

	void SetCollisionObject(std::shared_ptr<BulletGhostObject> shape);
private:
	std::shared_ptr<BulletGhostObject> mCollision;

	std::list<int> mTargetTags;
};

