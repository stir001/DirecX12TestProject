#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <memory>
#include <list>

class CollisionAction : public btActionInterface
{
public:
	CollisionAction();
	~CollisionAction();

	virtual void Action(int tag) = 0;

	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	void debugDraw(btIDebugDraw* debugDrawer);

	void AddTargetTag(int target);

	void RemoveTargetTag(int target);

	const std::list<int>& GetTargetTags() const;
private:
	std::shared_ptr<btGhostObject> mGhost;

	std::list<int> mTargetTags;
};

