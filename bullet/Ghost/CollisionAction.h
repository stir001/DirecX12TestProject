#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <list>

enum class BulletShapeType;
class BulletCollisionShape;
class btGhostObject;

class CollisionAction : public btActionInterface
{
public:
	CollisionAction(std::shared_ptr<BulletCollisionShape> shape);
	virtual ~CollisionAction();

	virtual void Action(int tag) = 0;

	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	void debugDraw(btIDebugDraw* debugDrawer);

	void AddTargetTag(int target);

	void RemoveTargetTag(int target);

	const std::list<int>& GetTargetTags() const;

	void SetCollisionShape(std::shared_ptr<BulletCollisionShape> shape);
private:
	std::shared_ptr<btGhostObject> mGhost;
	std::shared_ptr<BulletCollisionShape>  mCollisionShape;

	std::list<int> mTargetTags;
};

