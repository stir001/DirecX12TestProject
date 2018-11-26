#include "BoxRigidBody.h"



BoxRigidBody::BoxRigidBody(float length
	, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btBoxShape>(btVector3(length, length, length));
	CreateRigidBody();
	Translate(pos);
}


BoxRigidBody::~BoxRigidBody()
{
}
