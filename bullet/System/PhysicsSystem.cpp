#include "PhysicsSystem.h"
#include "BulletDebugDrawDx.h"
#include "Master/Dx12Ctrl.h"
#include "bullet/RigidBody/BulletRigidBody.h"
#include <btBulletDynamicsCommon.h>
#include <ctime>

PhysicsSystem::PhysicsSystem()
{
	//広域位相アルゴリズムの実装をインスタンス化
	mBroadphase = std::make_shared<btDbvtBroadphase>();
	
	//狭域位相アルゴリズムの実装をインスタンス化
	mCollisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
	mDispatcher = std::make_shared<btCollisionDispatcher>(mCollisionConfiguration.get());

	//ソルバー
	mSolver = std::make_shared<btSequentialImpulseConstraintSolver>();

	//設定を適応した世界を作る
	mWorld = std::make_shared<btDiscreteDynamicsWorld>(mDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get());

	//重力を設定する
	mWorld->setGravity(btVector3(0.f, -9.8f, 0.f));	

	mDebugDrawer = std::make_shared<BulletDebugDrawDx>(Dx12Ctrl::Instance().GetDev());
	mDebugDrawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
	mWorld->setDebugDrawer(mDebugDrawer.get());

	mTime = clock();
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::DebugDraw()
{
	mWorld->debugDrawWorld();
	mDebugDrawer->DebugDraw();
}

void PhysicsSystem::ClearDebugDraw()
{
	mDebugDrawer->ClearLines();
}

void PhysicsSystem::AddRigidBody(std::shared_ptr<BulletRigidBody> collider)
{
	mWorld->addRigidBody(collider->GetRigidBody().get());
}

void PhysicsSystem::Simulation()
{
	long currentTime = clock();
	mWorld->stepSimulation(currentTime - mTime);
	mTime = currentTime;
}

void PhysicsSystem::RemoveRigidBody(std::shared_ptr<BulletRigidBody> collider)
{
	mWorld->removeRigidBody(collider->GetRigidBody().get());
}
