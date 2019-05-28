#include "MovePrimitivePhase.h"
#include "GameObject.h"
#include "ChosePhase.h"
#include "PhaseChanger.h"
#include "LerpCalculator.h"

MovePrimitivePhase::MovePrimitivePhase(PhaseChanger& changer, std::shared_ptr<GameObject>& base
	, std::shared_ptr<GameObject>& plus, std::shared_ptr<GameObject>& minus)
	: Phase(changer), mBase(base), mPlus(plus), mMinus(minus)
	, mFrameCounter(0U), mLerp(nullptr), mUpdateState(nullptr)
{
}

MovePrimitivePhase::~MovePrimitivePhase()
{
}

void MovePrimitivePhase::Update(const DxInput& input)
{
	(this->*mUpdateState)(input);
}

void MovePrimitivePhase::Draw()
{
	mPlus->Draw();
	mMinus->Draw();
}

void MovePrimitivePhase::Initialize()
{
	mFirstPlusVal = mPlus->GetVelocity();
	mFirstMinusVal = mMinus->GetVelocity();
	mGoalFrame = 20U;
	mLerp = std::make_shared<LerpCalculator>();
	mUpdateState = &MovePrimitivePhase::UpdateCutMove;
}

void MovePrimitivePhase::Terminate()
{
}

void MovePrimitivePhase::UpdateCutMove(const DxInput & input)
{
	if (mFrameCounter <= mGoalFrame)
	{
		float rate = static_cast<float>(mFrameCounter) / static_cast<float>(mGoalFrame);
		auto plusVel = mLerp->Slerp(mFirstPlusVal, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), rate);
		auto minusVel = mLerp->Slerp(mFirstMinusVal, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), rate);
		mPlus->SetVelocity(plusVel);
		mMinus->SetVelocity(minusVel);
	}
	else
	{
		auto zeroVel = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mPlus->SetVelocity(zeroVel);
		mMinus->SetVelocity(zeroVel);
	}

	++mFrameCounter;

	if (mFrameCounter >= static_cast<unsigned int>(mGoalFrame * 1.2f))
	{
		mUpdateState = &MovePrimitivePhase::UpdateChoseMove;
		mGoalFrame = static_cast<unsigned int>(mGoalFrame * 1.5f);
		mFrameCounter = 0;
		mFirstPlusVal = mPlus->GetPos();
		mFirstMinusVal = mMinus->GetPos();
	}

	mPlus->Update();
	mMinus->Update();
}

void MovePrimitivePhase::UpdateChoseMove(const DxInput & input)
{
	if (mFrameCounter <= mGoalFrame)
	{
		float rate = static_cast<float>(mFrameCounter) / static_cast<float>(mGoalFrame);
		float x = 10.0f;
		auto plusPos = mLerp->Lerp(mFirstPlusVal, DirectX::XMFLOAT3(x, 0.0f, 0.0f), rate);
		auto minusPos = mLerp->Lerp(mFirstMinusVal, DirectX::XMFLOAT3(-x, 0.0f, 0.0f), rate);
		mPlus->SetPos(plusPos);
		mMinus->SetPos(minusPos);
		++mFrameCounter;
	}
	else
	{
		auto phase = std::make_shared<ChosePhase>(mChanger, mBase, mPlus, mMinus);
		mChanger.ChangePhase(phase);
	}
}
