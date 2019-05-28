#pragma once
#include "Phase.h"
#include <memory>
#include <vector>
#include <DirectXMath.h>

class GameObject;
class PrimitiveCutter;
struct PrimitiveVertex;

/**
*	切断するためのフェーズ
*/
class WaitCutPhase :
	public Phase
{
public:
	WaitCutPhase(PhaseChanger& changer,std::shared_ptr<GameObject>& baseObj);
	virtual ~WaitCutPhase();

	/**
	*	@brief	ゲームの更新
	*/
	void Update(const DxInput& input);

	/**
	*	@brief	描画
	*/
	void Draw();

	/**
	*	@brief	初期化処理
	*/
	void Initialize();

	/**
	*	@brief	終了処理
	*/
	void Terminate();
private:
	/**
	*	@brief	プリミティブを切断する
	*	@param[in]	origin	切断する際の平面の通る点
	*	@param[in]	normal	切断する平面の法線
	*/
	void Cut(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& normal);

	/**
	*	@brief	全ての頂点座標を平均値を原点、中心とした座標に再計算する
	*	@param[in,out]	再計算する頂点群
	*	@return			原点とした平均値
	*/
	DirectX::XMFLOAT3 RePlaceVertsPos(std::vector<PrimitiveVertex>& verts);

	/**
	*	@brief	切断するための原点と法線を取得する
	*	@return	原点、法線
	*/
	std::tuple<DirectX::XMFLOAT3, DirectX::XMFLOAT3> GetCutFace() const;

	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<GameObject> mPlus;
	std::shared_ptr<GameObject> mMinus;
	std::unique_ptr<PrimitiveCutter> mCutter;
};

