#include "PlayerSH.h"
#include "ImageController.h"
#include "DxInput.h"
#include "XMFloat3Operators.h"
#include "ActionStructs.h"
#include<iostream>
#include <algorithm>

const float VELOCITY_X  = 2.0f;
const float VELOCITY_Y  = 10.0f;
const float GRAVITY		= -1.0f;

PlayerSH::PlayerSH(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<DxInput> dlibInput) :IDrawableObject::IDrawableObject(imgCtrl)
, mInput(dlibInput), mVel(0.0f, 0.0f, 0.0f), mPos(0, 0, 0),mActionUpdate(&PlayerSH::Neutral), mIsturn(false), mFrame(0), mActionImageIndex(0)
{
	mImgCtrl->SetPos(mPos);
	mImgCtrl->SetScale(2.0f);
	mChangeNextAction = [&]() {
		ChangeAction(mCurrentAction->actionName.data());
	};
}

PlayerSH::~PlayerSH()
{
}

void PlayerSH::Update()
{
	(this->*mActionUpdate)();
	Gravity();
}

void PlayerSH::Draw()
{
	mImgCtrl->Draw();
}

const DirectX::XMFLOAT3& PlayerSH::GetPlayerPos() const
{
	return mPos;
}

void PlayerSH::OnGround(float grandLine)
{
	mPos.y = grandLine;
	if (mCurrentAction->actionName == "Jump")
	{
		ChangeAction("Ground");
		mActionUpdate = &PlayerSH::Ground;
	}
	mVel.y = 0;
}

void PlayerSH::SetAction(std::vector<Action>& inActs)
{
	mActions.swap(inActs);
	mCurrentAction = mActions.begin();
	mFrame = 0;
	mActionImageIndex = 0;
	SetActionImageData();
}

void PlayerSH::ChangeAction(const char* actionName)
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
	mImgCtrl->SetCenterOffset((mCurrentAction->datas[mActionImageIndex].pivot.x - imgSize.x / 2.0f), (-mCurrentAction->datas[mActionImageIndex].pivot.y + imgSize.y / 2.0f), 0);
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
				ChangeAction(mCurrentAction->actionName.data());
			}
			else
			{
				mChangeNextAction();
				SetActionImageData();
			}
		}
		else
		{
			mFrame = 0;
			SetActionImageData();
		}
	}
}

void PlayerSH::Gravity()
{
	mVel.y += GRAVITY;
}

void PlayerSH::Walk()
{
	mChangeNextAction = []() {};
	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_RIGHT))
	{
		mVel.x = VELOCITY_X;
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
	}
	else if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
	{
		mVel.x = -VELOCITY_X;
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
	}
	else
	{
		mVel.x = 0;
		mActionUpdate = &PlayerSH::Neutral;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
	{
		mVel.y = VELOCITY_Y;
		ChangeAction("Jump");
		mActionUpdate = &PlayerSH::Jump;
	}

	mPos += mVel;

	mImgCtrl->SetPos(mPos);
	AnimationUpdate();
}

void PlayerSH::Neutral()
{
	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_D))
	{
		mVel.x = VELOCITY_X;
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
		ChangeAction("Walk");
		mActionUpdate = &PlayerSH::Walk;
	}
	else if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_A))
	{
		mVel.x = -VELOCITY_X;
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
		ChangeAction("Walk");
		mActionUpdate = &PlayerSH::Walk;
	}
	else
	{
		mVel.x = 0;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
	{
		mVel.y = VELOCITY_Y;
		ChangeAction("Jump");
		mActionUpdate = &PlayerSH::Jump;
	}

	mPos += mVel;

	mImgCtrl->SetPos(mPos);
}

void PlayerSH::Jump()
{
	mChangeNextAction = [&]()
	{
		--mActionImageIndex;
		--mFrame;
	};
	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_D))
	{
		mVel.x = VELOCITY_X * 0.8f;
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_A))
	{
		mVel.x = -VELOCITY_X * 0.8f;
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
	}
	mPos += mVel;

	AnimationUpdate();
}

void PlayerSH::Ground()
{
	mVel.x = 0;
	mChangeNextAction = [&]() {
		if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN))
		{
			ChangeAction("Crouch");
			mActionUpdate = &PlayerSH::Crouch;
		}
		else
		{
			ChangeAction("Walk");
			mActionUpdate = &PlayerSH::Neutral;
		}
	};

	AnimationUpdate();
}

void PlayerSH::Crouch()
{
	mVel.x = 0;
	mChangeNextAction = [&]()
	{
		--mActionImageIndex;
		--mFrame;
	};

	if (!mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN))
	{
		ChangeAction("Walk");
		mActionUpdate = &PlayerSH::Neutral;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_D))
	{
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_A))
	{
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
	}

	AnimationUpdate();
}