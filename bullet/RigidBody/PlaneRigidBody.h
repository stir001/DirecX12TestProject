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
	*	@param[in]	constans	plane�萔(��)
	*	@param[in]	normal		�@��
	*	@param[in]	pos			�����ʒu
	*/
	PlaneRigidBody(float constans
		, const DirectX::XMFLOAT3& normal = DirectX::XMFLOAT3(0.f, 1.f, 0.f)
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~PlaneRigidBody();
private:

};

