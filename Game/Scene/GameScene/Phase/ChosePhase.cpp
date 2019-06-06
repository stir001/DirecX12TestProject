#include "ChosePhase.h"
#include "PhaseChanger.h"
#include "WaitCutPhase.h"
#include <Game/GameObject/GameObject.h>
#include <Game/Utillity/LerpCalculator.h>
#include <Dx12MSLib.h>

ChosePhase::ChosePhase(PhaseChanger& changer, std::shared_ptr<GameObject> base
	, std::shared_ptr<GameObject> plus, std::shared_ptr<GameObject> minus)
	: Phase(changer), mBase(base), mPlus(plus), mMinus(minus), mUpdateState(nullptr), mDrawState(nullptr)
	, mFrameCounter(0U), mGoalFrame(0U), mLerp(nullptr), mFirstVal{0.0f,0.0f,0.0f}
{
}


ChosePhase::~ChosePhase()
{
}

void ChosePhase::Update(const DxInput & input)
{
	(this->*mUpdateState)(input);
}

void ChosePhase::Draw()
{
	(this->*mDrawState)();
}

void ChosePhase::Initialize()
{
	mUpdateState = &ChosePhase::UpdateChose;
	mDrawState = &ChosePhase::DrawChose;
	mLerp = std::make_unique<LerpCalculator>();
}

void ChosePhase::Terminate()
{
}

void ChosePhase::UpdateChose(const DxInput& input)
{
	if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_RIGHT))
	{
		mBase = mPlus;
		ChangeMoveOrigin();
	}
	else if (input.IsKeyTrigger(VIRTUAL_KEY_INDEX::KEY_LEFT))
	{
		mBase = mMinus;
		ChangeMoveOrigin();
	}
}

void ChosePhase::UpdateMoveOrigin(const DxInput& input)
{
	if (mFrameCounter <= mGoalFrame)
	{
		float rate = static_cast<float>(mFrameCounter) / static_cast<float>(mGoalFrame);
		auto pos = mLerp->Lerp(mFirstVal, { 0.0f,0.0f,0.0f }, rate);
		mBase->SetPos(pos);
		++mFrameCounter;
	}
	else
	{
		mBase->SetPos({ 0.0f,0.0f,0.0f });
		auto phase = std::make_shared<WaitCutPhase>(mChanger, mBase);
		mChanger.ChangePhase(phase);
	}
}

void ChosePhase::ChangeMoveOrigin()
{
	mFirstVal = mBase->GetPos();
	mGoalFrame = 30U;
	mFrameCounter = 0U;
	mUpdateState = &ChosePhase::UpdateMoveOrigin;
	mDrawState = &ChosePhase::DrawMoveOrigin;
}

void ChosePhase::DrawChose()
{
	mPlus->Draw();
	mMinus->Draw();
}

void ChosePhase::DrawMoveOrigin()
{
	mBase->Draw();
}
