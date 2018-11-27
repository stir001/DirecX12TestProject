#pragma once
/**
*	@file BoxRigidBody.h
*	@brief �����RigidBody��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/27
*/
#include "BulletRigidBody.h"

/**
*	@class BoxRigidBody
*	@brief �����RigidBody
*/
class BoxRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	length	�ӂ̒���
	*	@param[in]	pos		�����ʒu
	*/
	BoxRigidBody(const DirectX::XMFLOAT3& length
	, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~BoxRigidBody();
private:
};

