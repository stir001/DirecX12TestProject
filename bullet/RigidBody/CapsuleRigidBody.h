#pragma once
/**
*	@file CapsuleRigidBody.h
*	@brief カプセル形状のRigidBody定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/26
*/
#include "BulletRigidBody.h"

/**
*	@class CapsuleRigidBody
*	@brief カプセル形状のRigidBody
*/
class CapsuleRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	半径
	*	@param[in]	height	高さ
	*	@param[in]	pos		初期位置
	*/
	CapsuleRigidBody(float radius, float height
	, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~CapsuleRigidBody();
private:
};

