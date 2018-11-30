#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <list>

enum class BulletShapeType;
class BulletGhostObject;
class BulletCollisionShape;

class CollisionAction : public btActionInterface
{
public:
	CollisionAction();
	virtual ~CollisionAction();

	virtual void Action(int tag) = 0;

	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	virtual void debugDraw(btIDebugDraw* debugDrawer);

	void AddTargetTag(int target);

	void RemoveTargetTag(int target);

	const std::list<int>& GetTargetTags() const;

	void SetCollisionObject(std::shared_ptr<BulletGhostObject> ghost);

	void SetShape(std::shared_ptr<BulletCollisionShape> shape);

	void SetCollisionState(int state);

	void Translate(float x, float y, float z);
protected:
	std::shared_ptr<BulletGhostObject> mCollision;

	std::list<int> mTargetTags;
};

