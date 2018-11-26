#include "BulletRigidBody.h"
#include "bullet/System/PhysicsSystem.h"


BulletRigidBody::BulletRigidBody() : mMass(1.0f)
{
}

BulletRigidBody::BulletRigidBody(std::shared_ptr<btCollisionShape> collisionShape
	, const DirectX::XMFLOAT3& pos)
	: mCollisionShape(collisionShape)
	, mMass(1.0f)
{
	CreateRigidBody();
	Translate(pos.x, pos.y, pos.z);
}


BulletRigidBody::~BulletRigidBody()
{
}

std::shared_ptr<btRigidBody> BulletRigidBody::GetRigidBody()
{
	return mRigidBody;
}

void BulletRigidBody::SetMass(float mass)
{
	mMass = mass;
	btVector3 bodyInertia;
	mCollisionShape->calculateLocalInertia(mMass, bodyInertia);
	mRigidBody->setMassProps(mMass, bodyInertia);
}

void BulletRigidBody::Translate(float x, float y, float z)
{
	mRigidBody->translate(btVector3(x, y, z));
}

void BulletRigidBody::Translate(const DirectX::XMFLOAT3 & pos)
{
	Translate(pos.x, pos.y, pos.z);
}

void BulletRigidBody::CreateRigidBody()
{
	mMotionState = std::make_shared<btDefaultMotionState>();
	btVector3 bodyInertia;
	mCollisionShape->calculateLocalInertia(mMass, bodyInertia);
	auto bodyCI = btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState.get(), mCollisionShape.get(), bodyInertia);

	bodyCI.m_restitution = 1.0f;
	bodyCI.m_friction = 0.5f;

	mRigidBody = std::make_shared<btRigidBody>(bodyCI);
}


