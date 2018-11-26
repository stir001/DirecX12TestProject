#include "CapsuleRigidBody.h"



CapsuleRigidBody::CapsuleRigidBody(float radius, float height
	, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btCapsuleShape>(radius, height);
	CreateRigidBody();
	Translate(pos);
}


CapsuleRigidBody::~CapsuleRigidBody()
{
}