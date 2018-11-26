#pragma once
#include <btBulletDynamicsCommon.h>
#include <memory>

class PhysicsSystem;

class Collider : public std::enable_shared_from_this<Collider>
{
public:
	Collider();
	virtual ~Collider();

	virtual std::shared_ptr<btRigidBody> GetRigidBody();

	virtual void SetPhysicsSystem(std::shared_ptr<PhysicsSystem> system);
protected:
	virtual void CreateRigidBody(btCollisionShape* collision);

	std::shared_ptr<btRigidBody> mRigidBody;
	std::shared_ptr<btMotionState> mMotionState;
	std::weak_ptr<PhysicsSystem> mPSystem;
	int mTag;
	btScalar mMass;
	bool mIsConvex;
};

