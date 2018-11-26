#pragma once
/**
*	@file SphereRigidBody.h
*	@brief �����RigidBody��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/26
*/
#include "BulletRigidBody.h"

/**
*	@class SphereRigidBody
*	@brief �����RigidBody
*/
class SphereRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	���a
	*	@param[in]	pos		�����ʒu
	*/
	SphereRigidBody(float radius
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~SphereRigidBody();
private:

};

