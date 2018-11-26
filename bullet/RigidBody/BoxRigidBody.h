#pragma once
#include "BulletRigidBody.h"
class BoxRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	length	�ӂ̒���
	*	@param[in]	pos		�����ʒu
	*/
	BoxRigidBody(float length
	, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~BoxRigidBody();
private:
};

