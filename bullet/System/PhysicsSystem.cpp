#include "PhysicsSystem.h"
#include "BulletDebugDrawDx.h"
#include "Master/Dx12Ctrl.h"
#include "bullet/RigidBody/BulletRigidBody.h"
#include "bullet/RigidBody/BoxRigidBody.h"
#include "bullet/RigidBody/SphereRigidBody.h"
#include "bullet/RigidBody/CylinderRigidBody.h"
#include "bullet/RigidBody/CapsuleRigidBody.h"
#include "bullet/RigidBody/PlaneRigidBody.h"
#include "bullet/RigidBody/ConeRigidBody.h"

#include <btBulletDynamicsCommon.h>
#include <ctime>
#include <algorithm>

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
	for (auto rigid : mRigidBodies)
	{
		mWorld->removeRigidBody(rigid.second->GetRigidBody().get());
	}
	mRigidBodies.clear();
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

void PhysicsSystem::AddRigidBody(std::shared_ptr<BulletRigidBody> rigid)
{
	if (rigid->GetTag() != -1) return;
	int key = 0;

	for (auto itr = mRigidBodies.begin(); itr != mRigidBodies.end(); ++itr)
	{
		if ((*itr).first != key)
		{
			auto fitr = std::find_if(itr, mRigidBodies.end(), [key](std::pair<const int, std::shared_ptr<BulletRigidBody>> data) { return data.first == key; });
			if (fitr == mRigidBodies.end())
			{
				break;
			}
		}
		++key;
	}

	mRigidBodies[key] = rigid;
	rigid->mTag = key;
	mWorld->addRigidBody(rigid->GetRigidBody().get());
}

void PhysicsSystem::Simulation()
{
	long currentTime = clock();
	mWorld->stepSimulation(currentTime - mTime);
	mTime = currentTime;
}

void PhysicsSystem::RemoveRigidBody(std::shared_ptr<BulletRigidBody> rigid)
{
	if (rigid->GetTag() == -1)
	{
		return;
	}
	auto fitr = mRigidBodies.find(rigid->GetTag());
	if (fitr == mRigidBodies.end())
	{
		return;
	}
	mRigidBodies.erase(fitr);
	mWorld->removeRigidBody(rigid->GetRigidBody().get());
}

void PhysicsSystem::RemoveRigidBody(int tag)
{
	auto itr = mRigidBodies.find(tag);
	if (itr == mRigidBodies.end())
	{
		return;
	}
	mRigidBodies.erase(itr);
	mWorld->removeRigidBody((*itr).second->GetRigidBody().get());
}

std::shared_ptr<BulletRigidBody> PhysicsSystem::CreateRigitBody(const BulletShapeType type, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos)
{
	std::shared_ptr<BulletRigidBody> rtn;
	switch (type)
	{
	case BulletShapeType::BOX:
		rtn = std::make_shared<BoxRigidBody>(data, pos);
		break;
	case BulletShapeType::SPHERE:
		rtn = std::make_shared<SphereRigidBody>(data.x, pos);
		break;
	case BulletShapeType::CYLINDER:
		rtn = std::make_shared<CylinderRigidBody>(data.x, data.y, pos);
		break;
	case BulletShapeType::CAPSULE:
		rtn = std::make_shared<CapsuleRigidBody>(data.x, data.y, pos);
		break;
	case BulletShapeType::PLANE:
		rtn = std::make_shared<PlaneRigidBody>(data.x, DirectX::XMFLOAT3(0.f, 1.f, 0.f), pos);
		break;
	case BulletShapeType::CONE:
		rtn = std::make_shared<ConeRigidBody>(data.x, data.y, pos);
		break;
	default:
		break;
	}

	AddRigidBody(rtn);

	return rtn;
}
