#include "SphereCollider.h"



SphereCollider::SphereCollider(float radius)
{
	mSphereShape = std::make_shared<btSphereShape>(radius);
	CreateRigidBody(mSphereShape.get());
}


SphereCollider::~SphereCollider()
{
}
