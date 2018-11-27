#include "ConeRigidBody.h"



ConeRigidBody::ConeRigidBody(float radius, float height, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btConeShape>(radius, height);
	CreateRigidBody();
	Translate(pos);
}


ConeRigidBody::~ConeRigidBody()
{
}
