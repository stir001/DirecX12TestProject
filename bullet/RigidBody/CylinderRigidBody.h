#pragma once
/**
*	@file CylinderRigidBody.h
*	@brief �����RigidBody��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/27
*/
#include "BulletRigidBody.h"

/**
*	@class	CylinderRigidBody
*	@brief	���`���RigidBody
*/
class CylinderRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	���a
	*	@param[in]	height	����
	*	@param[in]	pos		�����ʒu
	*/
	CylinderRigidBody(float radius, float height, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f,0.f,0.f));
	~CylinderRigidBody();
};

