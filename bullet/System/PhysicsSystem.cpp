#include "PhysicsSystem.h"
#include "BulletDebugDrawDx.h"
#include "Master/Dx12Ctrl.h"
#include "bullet/Collider/Collider.h"
#include <btBulletDynamicsCommon.h>
#include <ctime>

PhysicsSystem::PhysicsSystem()
{
	//�L��ʑ��A���S���Y���̎������C���X�^���X��
	mBroadphase = std::make_shared<btDbvtBroadphase>();
	
	//����ʑ��A���S���Y���̎������C���X�^���X��
	mCollisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
	mDispatcher = std::make_shared<btCollisionDispatcher>(mCollisionConfiguration.get());

	//�\���o�[
	mSolver = std::make_shared<btSequentialImpulseConstraintSolver>();

	//�ݒ��K���������E�����
	mWorld = std::make_shared<btDiscreteDynamicsWorld>(mDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get());

	//�d�͂�ݒ肷��
	mWorld->setGravity(btVector3(0.f, -9.8f, 0.f));	

	mDebugDrawer = new BulletDebugDrawDx(Dx12Ctrl::Instance().GetDev());
	mDebugDrawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
	mWorld->setDebugDrawer(mDebugDrawer);

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

void PhysicsSystem::AddRigidBody(std::shared_ptr<Collider> collider)
{
	mWorld->addRigidBody(collider->GetRigidBody().get());
	collider->SetPhysicsSystem(shared_from_this());
}

void PhysicsSystem::Simulation()
{
	long currentTime = clock();
	mWorld->stepSimulation(currentTime - mTime);
	mTime = currentTime;
}

void PhysicsSystem::RemoveRigidBody(std::shared_ptr<Collider> collider)
{
	mWorld->removeRigidBody(collider->GetRigidBody().get());
}
