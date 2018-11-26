#pragma once
/**
*	@file	PlaneRigidBody.h
*	@brief	�����`�Ŕ��RigidBody��`
*
*	@author	�^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/26
*/
#include "BulletRigidBody.h"

/**
*	@class	PlaneRigidBody
*	@brief	�����`�Ŕ��RigidBody
*/
class PlaneRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	length	��ӂ̒���
	*	@param[in]	pos		�����ʒu
	*/
	PlaneRigidBody(float length
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~PlaneRigidBody();
private:

};

