#pragma once
/**
*	@file BulletRigidBody.h
*	@brief btRigidBodyのラッパークラス定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/26
*/
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <DirectXMath.h>

/**
*	@class BulletRigidBody
*	@brief btRigidBodyのラッパークラス
*/
class BulletRigidBody : public std::enable_shared_from_this<BulletRigidBody>
{
public:
	BulletRigidBody();
	/**
	*	@param[in]	collisionShape	rigidBodyに設定する形状
	*	@param[in]	pos		初期位置
	*/
	BulletRigidBody(std::shared_ptr<btCollisionShape> collisionShape
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~BulletRigidBody();

	/**
	*	btRigidBodyを取得する
	*/
	virtual std::shared_ptr<btRigidBody> GetRigidBody();

	/**
	*	@brief	質量を設定する
	*	@param[in]	mass	設定する質量
	*/
	virtual void SetMass(float mass);

	/**
	*	@brief 位置を設定する	
	*			瞬間移動なので当たり判定時に不具合が起きる可能性あり
	*	@param[in]	x	x座標
	*	@param[in]	y	y座標
	*	@param[in]	z	z座標
	*/
	virtual void Translate(float x, float y, float z);

	/**
	*	@brief 位置を設定する
	*			瞬間移動なので当たり判定時に不具合が起きる可能性あり
	*	@param[in]	pos	設定する座標
	*/
	virtual void Translate(const DirectX::XMFLOAT3& pos);
protected:
	/**
	*	btRigidBodyを作成する
	*/
	virtual void CreateRigidBody();

	/**
	*	rigidBody
	*/
	std::shared_ptr<btRigidBody> mRigidBody;

	/**
	*	collisionの形状
	*/
	std::shared_ptr<btCollisionShape> mCollisionShape;

	/**
	*	モーションステート(理解不足)
	*/
	std::shared_ptr<btMotionState> mMotionState;

	/**
	*	タグ(理解不足)
	*/
	int mTag;

	/**
	*	質量
	*/
	btScalar mMass;
};

