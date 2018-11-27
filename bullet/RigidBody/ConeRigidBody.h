#pragma once
/**
*	@file ConeRigidBody.h
*	@brief �R�[�����RigidBody��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/27
*/
#include "BulletRigidBody.h"

/**
*	@class ConeRigidBody
*	@brief	�R�[���`���RigidBody
*/
class ConeRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	���a
	*	@param[in]	height	����
	*	@param[in]	pos		�����ʒu
	*/
	ConeRigidBody(float radius, float height, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~ConeRigidBody();
};

