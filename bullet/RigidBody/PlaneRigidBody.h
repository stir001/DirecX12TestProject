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
	*	@param[in]	constans	plane定数(謎)
	*	@param[in]	normal		法線
	*	@param[in]	pos			初期位置
	*/
	PlaneRigidBody(float constans
		, const DirectX::XMFLOAT3& normal = DirectX::XMFLOAT3(0.f, 1.f, 0.f)
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~PlaneRigidBody();
private:

};

