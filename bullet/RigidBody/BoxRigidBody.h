#pragma once
#include "BulletRigidBody.h"
class BoxRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	length	ï”ÇÃí∑Ç≥
	*	@param[in]	pos		èâä˙à íu
	*/
	BoxRigidBody(float length
	, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~BoxRigidBody();
private:
};

