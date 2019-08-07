#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>

class PrimitiveController;
class PrimitiveObject;
struct PrimitiveVertex;

class GameObject
{
public:
	GameObject();
	GameObject(const std::shared_ptr<PrimitiveController>& ctrl, const std::shared_ptr<PrimitiveObject>& data);
	~GameObject();

	/**
	*	@brief	状態を更新する
	*/
	void Update();

	/**
	*	@brief	速度を設定する
	*	@param[in]	vel	設定する速度
	*/
	void SetVelocity(const DirectX::XMFLOAT3& vel);

	/**
	*	@brief	速度を取得する
	*	@return	現在の速度
	*/
	DirectX::XMFLOAT3 GetVelocity() const;

	/**
	*	@brief	座標を設定する
	*	@param[in]	pos	設定する座標
	*/
	void SetPos(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	座標を取得する
	*	@return	現在の座標
	*/
	DirectX::XMFLOAT3 GetPos() const;

	/**
	*	@brief	描画する
	*/
	void Draw() const;

	/**
	*	@brief	PrimitiveObjectを取得する
	*/
	std::shared_ptr<PrimitiveObject>& GetData();

	/**
	*	@brief	頂点情報を取得する(直接の変更不可)
	*	@return	頂点情報
	*/
	const std::vector<PrimitiveVertex>& GetVertices() const;

	/**
	*	@brief	インデックス情報を取得する
	*	@return	インデクス情報
	*/
	const std::vector<unsigned int>& GetIndices() const;

	/**
	*	@brief	PrimitiveControllerとPrimitiveObjectを再設定する
	*	@param[in]	ctrl	再設定するPrimitiveController
	*	@param[in]	data	再設定するPrimitiveObject
	*/
	void Reset(const std::shared_ptr<PrimitiveController>& ctrl, const std::shared_ptr<PrimitiveObject>& data);

	/**
	*	@brief	色を設定する
	*	@param[in]	設定する色
	*/
	void SetColor(const DirectX::XMFLOAT3& color);

	/**
	*	当たり判定用の球の半径を取得する
	*/
	float GetCollisionRadius() const;
private:
	/**
	*	当たり判定用球の半径を設定する
	*/
	void SetCollisionRadius();

	std::shared_ptr<PrimitiveController> mCtrl;
	std::shared_ptr<PrimitiveObject> mData;

	DirectX::XMFLOAT3 mVel;
	DirectX::XMFLOAT3 mPos;

	/**
	*	大まかな当たり判定時に用いる全体を囲む円の半径
	*/
	float mCollisionRadius;
};

