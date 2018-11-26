#pragma once
/**
*	@file	PlaneRigidBody.h
*	@brief	正方形で板状のRigidBody定義
*
*	@author	真鍋奨一郎
*
*	@par 最終更新日	2018/11/26
*/
#include "BulletRigidBody.h"

/**
*	@class	PlaneRigidBody
*	@brief	正方形で板状のRigidBody
*/
class PlaneRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	length	一辺の長さ
	*	@param[in]	pos		初期位置
	*/
	PlaneRigidBody(float length
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~PlaneRigidBody();
private:

};

