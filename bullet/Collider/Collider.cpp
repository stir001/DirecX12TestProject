#include "Collider.h"
#include "bullet/System/PhysicsSystem.h"


Collider::Collider()
{
}


Collider::~Collider()
{
}

std::shared_ptr<btRigidBody> Collider::GetRigidBody()
{
	return mRigidBody;
}

void Collider::SetPhysicsSystem(std::shared_ptr<PhysicsSystem> system)
{
	mPSystem = system;
}

void Collider::CreateRigidBody(btCollisionShape* collision)
{
	mMotionState = std::make_shared<btDefaultMotionState>();
	mMass = 1.0;
	btVector3 bodyInertia;
	collision->calculateLocalInertia(mMass, bodyInertia);
	auto bodyCI = btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState.get(), collision, bodyInertia);

	bodyCI.m_restitution = 1.0f;
	bodyCI.m_friction = 0.5f;

	mRigidBody = std::make_shared<btRigidBody>(bodyCI);
}
