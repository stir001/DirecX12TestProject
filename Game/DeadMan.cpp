#include "DeadMan.h"
#include"Dx12MSLib.h"
#include "PlayerSH.h"

const float VELOCITY_X = 0.5f;
const float RETURN_DISTANCE = 50.0f;
const int DEAD_LINE_COUNT = 60;

DeadMan::DeadMan(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<PlayerSH> spPlayer)
	: Enemy(imgCtrl,spPlayer), mDeadCount(0)
{
	mVel.x = VELOCITY_X;
	mActionUpdate = &DeadMan::Walk;
	mPos = { 0,0,0 };
	mImgCtrl->SetPos(mPos);
}

DeadMan::DeadMan(std::shared_ptr<ImageController>& imgCtrl, float x, float y, float z , std::shared_ptr<PlayerSH> spPlayer)
	: Enemy(imgCtrl, spPlayer), mDeadCount(0)
{
	mVel.x = VELOCITY_X;
	mActionUpdate = &DeadMan::Walk;
	mPos = { x, y, z };
	mImgCtrl->SetPos(mPos);
}

DeadMan::~DeadMan()
{
}

void DeadMan::Draw() const
{
	mImgCtrl->Draw();
}

void DeadMan::Update()
{
	(this->*mActionUpdate)();
	Gravity();
}

void DeadMan::OnGround(float grandLine)
{
	mPos.y = grandLine;
	mVel.y = 0;
	mImgCtrl->SetPos(mPos);
}

void DeadMan::OnDamage()
{
	ChangeAction("Damage");
	mActionUpdate = &DeadMan::Damage;
}

void DeadMan::Gravity()
{
	mVel.y += -0.8f;
}

void DeadMan::Walk()
{
	float sub = 0;
	if (fabsf(sub = (mwpPlayer.lock()->GetPos().x - mPos.x)) >= RETURN_DISTANCE)
	{
		if (sub <= 0)
		{
			if (!mIsturn)
			{
				mVel.x = -VELOCITY_X;
				mIsturn = true;
				mImgCtrl->TurnU();
			}
		}
		else
		{
			if (mIsturn)
			{
				mVel.x = VELOCITY_X;
				mIsturn = false;
				mImgCtrl->TurnU();
			}
		}
	}
	UpdatePostion();
	AnimationUpdate();
}

void DeadMan::Damage()
{
	mChangeNextAction = [&]() {
		ChangeAction("Die");
		mActionUpdate = &DeadMan::Die;
	};
	AnimationUpdate();
}

void DeadMan::Die()
{
	mChangeNextAction = [&]() {
		--mActionImageIndex;
		--mFrame;
		if (++mDeadCount > DEAD_LINE_COUNT)
		{
			mIsDead = true;
		}
	};

	AnimationUpdate();
}
