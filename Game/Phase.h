#pragma once

class DxInput;
class PhaseChanger;

/**
*	ゲームシーン中の段階を分けるための基底クラス
*/
class Phase
{
public:
	Phase(PhaseChanger& changer);
	virtual ~Phase();

	/**
	*	@brief	フェーズの更新
	*			毎フレーム呼ばれる
	*/
	virtual void Update(const DxInput& input) = 0;

	/**
	*	@brief	描画
	*			毎フレーム呼ばれる
	*/
	virtual void Draw() = 0;

	/**
	*	@brief	初期化処理
	*			PhaseChangerによって呼ばれる
	*/
	virtual void Initialize() = 0;

	/**
	*	@brief	終了処理
	*			PhaceChangerによって呼ばれる
	*/
	virtual void Terminate() = 0;
protected:
	PhaseChanger& mChanger;
};

