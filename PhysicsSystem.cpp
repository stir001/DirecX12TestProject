#include "PhysicsSystem.h"



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

	//���͂�ݒ肷��
	mWorld->setGravity(btVector3(0, -9.8, 0));	

}


PhysicsSystem::~PhysicsSystem()
{
}
