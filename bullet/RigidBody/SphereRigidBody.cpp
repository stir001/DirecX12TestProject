#include "SphereRigidBody.h"



SphereRigidBody::SphereRigidBody(float radius
	, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btSphereShape>(radius);
	CreateRigidBody();
	Translate(pos);
}


SphereRigidBody::~SphereRigidBody()
{
}

