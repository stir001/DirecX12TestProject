#include "PlaneRigidBody.h"



PlaneRigidBody::PlaneRigidBody(float constans
	, const DirectX::XMFLOAT3& normal
	, const DirectX::XMFLOAT3& pos)
{
	mCollisionShape = std::make_shared<btStaticPlaneShape>(btVector3(normal.x, normal.y, normal.z), constans);
	mMass = 0.0f;
	CreateRigidBody();
	Translate(pos);
}


PlaneRigidBody::~PlaneRigidBody()
{
}

