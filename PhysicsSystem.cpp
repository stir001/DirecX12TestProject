#include "PhysicsSystem.h"



PhysicsSystem::PhysicsSystem()
{
	//�L��ʑ��A���S���Y���̎������C���X�^���X��
	mBroadphase = new btDbvtBroadphase();
	
	//����ʑ��A���S���Y���̎������C���X�^���X��
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	//�\���o�[
	mSolver = new btSequentialImpulseConstraintSolver();

	//�ݒ��K���������E�����
	mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	//���͂�ݒ肷��
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
