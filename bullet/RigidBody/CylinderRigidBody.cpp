#include "CylinderRigidBody.h"



CylinderRigidBody::CylinderRigidBody(float radius, float height, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btCylinderShape>(btVector3(radius, height, radius));
	CreateRigidBody();
	Translate(pos);
}


CylinderRigidBody::~CylinderRigidBody()
{
}
