#pragma once
#include <btBulletDynamicsCommon.h>
#include <memory>

class BulletDebugDrawDx;
class Collider;

class PhysicsSystem : public std::enable_shared_from_this<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void DebugDraw();

	void ClearDebugDraw();

	void AddRigidBody(std::shared_ptr<Collider> collider);

	void Simulation();

	void RemoveRigidBody(std::shared_ptr<Collider> collider);
private:
	std::shared_ptr<btBroadphaseInterface>					mBroadphase;
	std::shared_ptr<btDefaultCollisionConfiguration>		mCollisionConfiguration;
	std::shared_ptr<btCollisionDispatcher>					mDispatcher;
	std::shared_ptr<btSequentialImpulseConstraintSolver>	mSolver;
	std::shared_ptr<btDiscreteDynamicsWorld>				mWorld;
	BulletDebugDrawDx* mDebugDrawer;
	long mTime;
};

