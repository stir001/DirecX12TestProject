#include "BoxRigidBody.h"



BoxRigidBody::BoxRigidBody(const DirectX::XMFLOAT3& length
	, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btBoxShape>(btVector3(length.x, length.y, length.z));
	CreateRigidBody();
	Translate(pos);
}


BoxRigidBody::~BoxRigidBody()
{
}
