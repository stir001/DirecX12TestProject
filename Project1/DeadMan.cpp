#include "DeadMan.h"
#include "ImageController.h"
#include "XMFloat3Operators.h"
#include "PlayerSH.h"

const float VELOCITY_X = 0.5f;
const float RETURN_DISTANCE = 50.0f;

DeadMan::DeadMan(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<PlayerSH> spPlayer):Enemy(imgCtrl,spPlayer)
{
	mVel.x = VELOCITY_X;
	mActionUpdate = &DeadMan::Walk;
	mPos = { 0,0,0 };
	mImgCtrl->SetPos(mPos);
}

DeadMan::DeadMan(std::shared_ptr<ImageController>& imgCtrl, float x, float y, float z , std::shared_ptr<PlayerSH> spPlayer):Enemy(imgCtrl, spPlayer)
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
				mImgCtrl->TurnX();
			}
		}
		else
		{
			if (mIsturn)
			{
				mVel.x = VELOCITY_X;
				mIsturn = false;
				mImgCtrl->TurnX();
			}
		}
	}
	UpdatePostion();
	AnimationUpdate();
}

void DeadMan::Damage()
{

}
