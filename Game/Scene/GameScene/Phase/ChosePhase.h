#pragma once
#include "Phase.h"
#include <memory>
#include <DirectXMath.h>

class PhaseChanger;
class GameObject;
class LerpCalculator;

class ChosePhase :
	public Phase
{
public:
	ChosePhase(PhaseChanger& changer, std::shared_ptr<GameObject> base
		, std::shared_ptr<GameObject> plus, std::shared_ptr<GameObject> minus);
	virtual ~ChosePhase();
	/**
	*	@brief	フェーズの更新
	*			毎フレーム呼ばれる
	*/
	 void Update(const DxInput& input);

	/**
	*	@brief	描画
	*			毎フレーム呼ばれる
	*/
	 void Draw();

	/**
	*	@brief	初期化処理
	*			PhaseChangerによって呼ばれる
	*/
	 void Initialize();

	/**
	*	@brief	終了処理
	*			PhaceChangerによって呼ばれる
	*/
	 void Terminate();
private:
	std::shared_ptr<GameObject> mBase;
	std::shared_ptr<GameObject> mPlus;
	std::shared_ptr<GameObject> mMinus;
	std::unique_ptr<LerpCalculator> mLerp;
	DirectX::XMFLOAT3 mFirstVal;
	unsigned int mFrameCounter;
	unsigned int mGoalFrame;

	void (ChosePhase::*mUpdateState)(const DxInput&);
	void (ChosePhase::*mDrawState)();

	void UpdateChose(const DxInput& input);
	void UpdateMoveOrigin(const DxInput& input);
	void ChangeMoveOrigin();

	void DrawChose();
	void DrawMoveOrigin();
};

