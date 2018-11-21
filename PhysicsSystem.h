#pragma once
#include <btBulletDynamicsCommon.h>
#include <memory>

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();
private:
	btBroadphaseInterface*					mBroadphase;
	btDefaultCollisionConfiguration*		mCollisionConfiguration;
	btCollisionDispatcher*					mDispatcher;
	btSequentialImpulseConstraintSolver*	mSolver;
	btDiscreteDynamicsWorld*				mWorld;
};

