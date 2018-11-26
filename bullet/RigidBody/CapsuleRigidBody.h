#pragma once
/**
*	@file CapsuleRigidBody.h
*	@brief �J�v�Z���`���RigidBody��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/26
*/
#include "BulletRigidBody.h"

/**
*	@class CapsuleRigidBody
*	@brief �J�v�Z���`���RigidBody
*/
class CapsuleRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	���a
	*	@param[in]	height	����
	*	@param[in]	pos		�����ʒu
	*/
	CapsuleRigidBody(float radius, float height
	, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~CapsuleRigidBody();
private:
};

