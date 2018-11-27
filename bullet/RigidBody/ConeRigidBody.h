#pragma once
/**
*	@file ConeRigidBody.h
*	@brief コーン状のRigidBody定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/27
*/
#include "BulletRigidBody.h"

/**
*	@class ConeRigidBody
*	@brief	コーン形状のRigidBody
*/
class ConeRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	radius	半径
	*	@param[in]	height	高さ
	*	@param[in]	pos		初期位置
	*/
	ConeRigidBody(float radius, float height, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~ConeRigidBody();
};

