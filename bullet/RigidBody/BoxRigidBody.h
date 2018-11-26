#pragma once
#include "BulletRigidBody.h"
class BoxRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	length	辺の長さ
	*	@param[in]	pos		初期位置
	*/
	BoxRigidBody(float length
	, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~BoxRigidBody();
private:
};

