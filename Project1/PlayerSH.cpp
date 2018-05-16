#include "PlayerSH.h"
#include "ImageController.h"
#include "DxInput.h"
#include "XMFloat3Operators.h"
#include "ActionStructs.h"
#include<iostream>
#include <algorithm>

const float VELOCITY_X  = 2.0f;
const float VELOCITY_Y  = 4.0f;
const float GRAVITY		= 1.0f;

PlayerSH::PlayerSH(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<DxInput> dlibInput) :IDrawableObject::IDrawableObject(imgCtrl)
, mInput(dlibInput), mVel(0.0f, 0.0f, 0.0f), mPos(0, 0, 0),mActionUpdate(&PlayerSH::Walk), mIsturn(false), mFrame(0), mActionImageIndex(0)
{
	mImgCtrl->SetPos(mPos);
	mImgCtrl->SetScale(2.0f);
	mChangeNextAction = [&]() {
		ChangeAction(mCurrentAction->actionName);
	};
}

PlayerSH::~PlayerSH()
{
}

void PlayerSH::Update()
{
	(this->*mActionUpdate)();
}

void PlayerSH::Draw()
{
	mImgCtrl->Draw();
}

void PlayerSH::Walk()
{
	mVel.x = 0;
	mVel.y = 0;
	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_D))
	{
		mVel.x = VELOCITY_X;
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_A))
	{
		mVel.x = -VELOCITY_X;
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
	}

	mVel.y = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_W) ?
		VELOCITY_Y : mVel.y;

	mVel.y = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_S) ?
		-VELOCITY_Y : mVel.y;

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
	{
		mImgCtrl->AddRota(1.0f);
	}

	if (mInput->IsKeyTrigger(eVIRTUAL_KEY_INDEX_UP))
	{
		mImgCtrl->TurnY();
	}

	mPos += mVel;

	mImgCtrl->SetPos(mPos);
	AnimationUpdate();
}

const DirectX::XMFLOAT3& PlayerSH::GetPlayerPos() const
{
	return mPos;
}

void PlayerSH::OnGround(float grandLine)
{
	mVel.y = 0;
	mPos.y = grandLine;
}

void PlayerSH::SetAction(std::vector<Action>& inActs)
{
	mActions.swap(inActs);
	mCurrentAction = mActions.begin();
	mFrame = 0;
	mActionImageIndex = 0;
	SetActionImageData();
}

void PlayerSH::ChangeAction(const std::string& actionName)
{
	auto itr = std::find_if(mActions.begin(), mActions.end(), [&](const Action& act) {return act.actionName == actionName; });
	if (itr != mActions.end())
	{
		mCurrentAction = itr;
		mFrame = 0;
		mActionImageIndex = 0;
		SetActionImageData();
	}
}

void PlayerSH::SetActionImageData()
{
	DirectX::XMFLOAT2 imgSize = { mCurrentAction->datas[mActionImageIndex].imageRect.GetWidth(),mCurrentAction->datas[mActionImageIndex].imageRect.GetHeight() };
	mImgCtrl->SetCenterOffset((imgSize.x / 2.0f - mCurrentAction->datas[mActionImageIndex].pivot.x), (mCurrentAction->datas[mActionImageIndex].pivot.y - imgSize.y / 2.0f), 0);
	mImgCtrl->SetRect(mCurrentAction->datas[mActionImageIndex].imageRect);
}

void PlayerSH::AnimationUpdate()
{
	if (++mFrame >= mCurrentAction->datas[mActionImageIndex].duraction)
	{
		if (++mActionImageIndex >= mCurrentAction->datas.size())
		{
			if (mCurrentAction->isLoop)
			{
				ChangeAction(mCurrentAction->actionName);
			}
			else
			{
				mChangeNextAction();
				SetActionImageData();
			}
		}
		else
		{
			++mActionImageIndex;
			mFrame = 0;
			SetActionImageData();
		}
	}
}

void PlayerSH::Neutral()
{
	mVel.x = 0;
	mVel.y = 0;
	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_D))
	{
		mVel.x = VELOCITY_X;
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_A))
	{
		mVel.x = -VELOCITY_X;
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
	}

	mVel.y = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_W) ?
		VELOCITY_Y : mVel.y;

	mVel.y = mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_S) ?
		-VELOCITY_Y : mVel.y;

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
	{
		mImgCtrl->AddRota(1.0f);
	}

	if (mInput->IsKeyTrigger(eVIRTUAL_KEY_INDEX_UP))
	{
		mImgCtrl->TurnY();
	}

	mPos += mVel;

	mImgCtrl->SetPos(mPos);
}
