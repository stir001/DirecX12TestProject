#include "CapsuleCollider.h"



CapsuleCollider::CapsuleCollider(float radius, float height)
{
	mCapsuleShape = std::make_shared<btCapsuleShape>(radius, height);
	CreateRigidBody(mCapsuleShape.get());
}


CapsuleCollider::~CapsuleCollider()
{
}
