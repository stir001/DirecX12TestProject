#include "PhysicsSystem.h"



PhysicsSystem::PhysicsSystem()
{
	//広域位相アルゴリズムの実装をインスタンス化
	mBroadphase = new btDbvtBroadphase();
	
	//狭域位相アルゴリズムの実装をインスタンス化
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	//ソルバー
	mSolver = new btSequentialImpulseConstraintSolver();

	//設定を適応した世界を作る
	mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	//呪力を設定する
	mWorld->setGravity(btVector3(0, -9.8, 0));	

}


PhysicsSystem::~PhysicsSystem()
{
	delete mSolver;
	delete mCollisionConfiguration;
	delete mDispatcher;
	delete mBroadphase;
	delete mWorld;
}
