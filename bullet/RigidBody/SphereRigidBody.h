#pragma once
/**
*	@file SphereRigidBody.h
*	@brief 球状のRigidBody定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/26
*/
#include "BulletRigidBody.h"

/**
*	@class SphereRigidBody
*	@brief 球状のRigidBody
*/
class SphereRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	半径
	*	@param[in]	pos		初期位置
	*/
	SphereRigidBody(float radius
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~SphereRigidBody();
private:

};

