#pragma once
/**
*	@file CylinderRigidBody.h
*	@brief 筒状のRigidBody定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/27
*/
#include "BulletRigidBody.h"

/**
*	@class	CylinderRigidBody
*	@brief	筒形状のRigidBody
*/
class CylinderRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	半径
	*	@param[in]	height	高さ
	*	@param[in]	pos		初期位置
	*/
	CylinderRigidBody(float radius, float height, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f,0.f,0.f));
	~CylinderRigidBody();
};

