#include "PlaneRigidBody.h"



PlaneRigidBody::PlaneRigidBody(float length
	, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btStaticPlaneShape>(btVector3(0.f, 1.0f, 0.0f), length);
	mMass = 0.0f;
	CreateRigidBody();
	Translate(pos);
}


PlaneRigidBody::~PlaneRigidBody()
{
}

