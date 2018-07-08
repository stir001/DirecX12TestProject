#include "Bat.h"
#include "ImageController.h"
#include "PlayerSH.h"
#include "XMFloatOperators.h"

const float VELOCITY_X = 0.3f;

Bat::Bat(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<PlayerSH> spPlayer) :Enemy(imgCtrl, spPlayer)
{
	mVel.x = VELOCITY_X;
	mActionUpdate = &Bat::Wait;
	mPos = { 0,0,0 };
	mImgCtrl->SetPos(mPos);
}

Bat::Bat(std::shared_ptr<ImageController>& imgCtrl, float x, float y, float z, std::shared_ptr<PlayerSH> spPlayer) : Enemy(imgCtrl, spPlayer)
{
	mVel.x = VELOCITY_X;
	mActionUpdate = &Bat::Wait;
	mPos = { x, y, z };
	mImgCtrl->SetPos(mPos);
}

Bat::~Bat()
{
}

void Bat::Draw() const
{
	mImgCtrl->Draw();
}

void Bat::Update()
{
	(this->*mActionUpdate)();
}

void Bat::OnGround(float groundLine)
{
	mPos.y = groundLine;
}

void Bat::OnDamage()
{
	mActionUpdate = &Bat::Damage;
	ChangeAction("Damage");
}

void Bat::Wait()
{
	if ((mwpPlayer.lock()->GetPos().x - mPos.x) <= 0 && mImgCtrl->IsTurnX())
	{
		mImgCtrl->TurnX();
		mVel.x *= -1;
	}

	if (abs(mwpPlayer.lock()->GetPos().x - mPos.x) < 100)
	{
		ChangeAction("Fly");
		mActionUpdate = &Bat::FlyDown;
	}
}

void Bat::Damage()
{
	mChangeNextAction = [&](){
		ChangeAction("Die");
	};

	AnimationUpdate();
}

void Bat::Die()
{
	mChangeNextAction = [&]() {
		--mFrame;
		--mActionImageIndex;
	};

	AnimationUpdate();
}

void Bat::Fly()
{
	mPos += mVel;
	AnimationUpdate();
}

void Bat::FlyDown()
{
	if (mPos.y > mwpPlayer.lock()->GetPos().y)
	{
		mPos.y -= 0.2f;
	}
	else
	{
		mActionUpdate = &Bat::Fly;
	}

	AnimationUpdate();
}

