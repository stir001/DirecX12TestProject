#include "PlayerSH.h"
#include "ImageController.h"
#include "DxInput.h"
#include "ActionStructs.h"


const float VELOCITY_X  = 2.0f;
const float VELOCITY_Y  = 20.0f;
const float GRAVITY		= -1.0f;

PlayerSH::PlayerSH(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<DxInput> dlibInput) :ICharactor::ICharactor(imgCtrl)
, mInput(dlibInput), mActionUpdate(&PlayerSH::Neutral)
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
		mFrame = 0;
		mActionImageIndex = 0;
		SetActionImageData();
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN))
	{
		mVel.x = 0;
		ChangeAction("Crouch");
		mActionUpdate = &PlayerSH::Crouch;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
	{
		mVel.y = VELOCITY_Y;
		ChangeAction("Jump");
		mActionUpdate = &PlayerSH::Jump;
	}
	else if (mInput->IsKeyTrigger(eVIRTUAL_KEY_INDEX_X))
	{
		mVel.x = 0;
		ChangeAction("Punch");
		mActionUpdate = &PlayerSH::Punch;
	}

	UpdatePostion();

	AnimationUpdate();
}

void PlayerSH::Neutral()
{
	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_RIGHT))
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
	else if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
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

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN))
	{
		mVel.x = 0;
		ChangeAction("Crouch");
		mActionUpdate = &PlayerSH::Crouch;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
	{
		mVel.y = VELOCITY_Y;
		ChangeAction("Jump");
		mActionUpdate = &PlayerSH::Jump;
	}
	else if(mInput->IsKeyTrigger(eVIRTUAL_KEY_INDEX_X))
	{
		mVel.x = 0;
		ChangeAction("Punch");
		mActionUpdate = &PlayerSH::Punch;
	}

	UpdatePostion();
}

void PlayerSH::Jump()
{
	mChangeNextAction = [&]()
	{
		--mActionImageIndex;
		--mFrame;
	};
	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_RIGHT))
	{
		mVel.x = VELOCITY_X * 0.8f;
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
	{
		mVel.x = -VELOCITY_X * 0.8f;
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_DOWN) && mInput->IsKeyTrigger(eVIRTUAL_KEY_INDEX_X))
	{
		ChangeAction("Kick");
		mActionUpdate = &PlayerSH::Kick;
		mChangeNextAction = [&]() 
		{
			ChangeAction("Jump");
			mActionImageIndex = static_cast<int>(mCurrentAction->datas.size() - 1);
			mFrame = mCurrentAction->datas[mActionImageIndex].duraction - 1;
		};

	}

	UpdatePostion();

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
	else if (mInput->IsKeyTrigger(eVIRTUAL_KEY_INDEX_X))
	{
		ChangeAction("Kick");
		mActionUpdate = &PlayerSH::Kick;
		mChangeNextAction = [&]()
		{
			ChangeAction("Crouch");
			mActionUpdate = &PlayerSH::Crouch;
		};
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_Z))
	{
		ChangeAction("Jump");
		mActionUpdate = &PlayerSH::Jump;
		mVel.y = VELOCITY_Y;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_RIGHT))
	{
		if (mIsturn == true)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = false;
	}

	if (mInput->IsKeyDown(eVIRTUAL_KEY_INDEX_LEFT))
	{
		if (mIsturn == false)
		{
			mImgCtrl->TurnX();
		}
		mIsturn = true;
	}

	UpdatePostion();

	AnimationUpdate();
}

void PlayerSH::Punch()
{
	mChangeNextAction = [&]()
	{
		ChangeAction("Walk");
		mActionUpdate = &PlayerSH::Neutral;
	};

	AnimationUpdate();
}

void PlayerSH::Kick()
{
	UpdatePostion();
	AnimationUpdate();
}
