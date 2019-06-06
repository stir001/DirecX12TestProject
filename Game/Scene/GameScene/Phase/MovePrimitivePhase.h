#pragma once
#include "Phase.h"
#include <memory>
#include <DirectXMath.h>

class PhaceChanger;
class GameObject;
class LerpCalculator;

/**
*	@class	切断されたプリミティブが既定の位置へ移動するフェーズ
*/
class MovePrimitivePhase :
	public Phase
{
public:
	MovePrimitivePhase(PhaseChanger& changer, std::shared_ptr<GameObject>& base
		, std::shared_ptr<GameObject>& plus, std::shared_ptr<GameObject>& minus);
	virtual ~MovePrimitivePhase();

	/**
	*	@brief	フェーズの更新
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
	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<GameObject> mPlus;
	std::shared_ptr<GameObject> mMinus;

	/**
	*	カウント用タイマー
	*/
	unsigned int mFrameCounter;

	/**
	*	最終目標フレーム
	*/
	unsigned int mGoalFrame;
	DirectX::XMFLOAT3 mFirstPlusVal;
	DirectX::XMFLOAT3 mFirstMinusVal;
	std::shared_ptr<LerpCalculator> mLerp;

	void (MovePrimitivePhase::*mUpdateState)(const DxInput& input);

	/**
	*	@brief	切断された後の移動更新状態
	*	@param@[in]	input	入力情報
	*/
	void UpdateCutMove(const DxInput& input);

	/**
	*	@brief	選択状態の位置への移動更新状態
	*	@param[in]	input	入力情報
	*/
	void UpdateChoseMove(const DxInput& input);
};

