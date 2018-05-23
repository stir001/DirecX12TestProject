#include "DeadMan.h"
#include"ImageController.h"


DeadMan::DeadMan(std::shared_ptr<ImageController>& imgCtrl):Enemy(imgCtrl)
{
	mImgCtrl->SetScale(2.0f);
}

DeadMan::DeadMan(std::shared_ptr<ImageController>& imgCtrl, float x, float y, float z):Enemy(imgCtrl)
{
	mImgCtrl->SetScale(2.0f);
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
	AnimationUpdate();
	Gravity();
}

void DeadMan::OnGround(float grandLine)
{
	mPos.y = grandLine;
	mVel.y = 0;
	mImgCtrl->SetPos(mPos);
}

void DeadMan::Gravity()
{
	mVel.y += -0.8f;
}

