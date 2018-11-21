#pragma once
#include <btBulletDynamicsCommon.h>
#include <memory>

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();
private:
	std::shared_ptr<btBroadphaseInterface>					mBroadphase;
	std::shared_ptr<btDefaultCollisionConfiguration>		mCollisionConfiguration;
	std::shared_ptr<btCollisionDispatcher>					mDispatcher;
	std::shared_ptr<btSequentialImpulseConstraintSolver>	mSolver;
	std::shared_ptr<btDiscreteDynamicsWorld>				mWorld;
};

