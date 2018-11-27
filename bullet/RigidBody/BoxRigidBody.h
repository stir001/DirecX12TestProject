#pragma once
/**
*	@file BoxRigidBody.h
*	@brief 箱状のRigidBody定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/27
*/
#include "BulletRigidBody.h"

/**
*	@class BoxRigidBody
*	@brief 箱状のRigidBody
*/
class BoxRigidBody :
	public BulletRigidBody
{
public:
	/**
	*	@param[in]	length	辺の長さ
	*	@param[in]	pos		初期位置
	*/
	BoxRigidBody(const DirectX::XMFLOAT3& length
	, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~BoxRigidBody();
private:
};

