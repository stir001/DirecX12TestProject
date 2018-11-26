#pragma once
#include <btBulletDynamicsCommon.h>
#include <memory>

class BulletDebugDrawDx;
class BulletRigidBody;

class PhysicsSystem : public std::enable_shared_from_this<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void DebugDraw();

	void ClearDebugDraw();

	void AddRigidBody(std::shared_ptr<BulletRigidBody> collider);

	void Simulation();

	void RemoveRigidBody(std::shared_ptr<BulletRigidBody> collider);
private:
	std::shared_ptr<btBroadphaseInterface>					mBroadphase;
	std::shared_ptr<btDefaultCollisionConfiguration>		mCollisionConfiguration;
	std::shared_ptr<btCollisionDispatcher>					mDispatcher;
	std::shared_ptr<btSequentialImpulseConstraintSolver>	mSolver;
	std::shared_ptr<btDiscreteDynamicsWorld>				mWorld;
	std::shared_ptr<BulletDebugDrawDx>						mDebugDrawer;
	long mTime;
};

