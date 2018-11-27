#include "BoxRigidBody.h"



BoxRigidBody::BoxRigidBody(const DirectX::XMFLOAT3& length
	, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btBoxShape>(btVector3(length.x * 0.5f, length.y * 0.5f, length.z * 0.5f));
	CreateRigidBody();
	Translate(pos);
}


BoxRigidBody::~BoxRigidBody()
{
}
