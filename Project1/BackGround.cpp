#include "BackGround.h"
#include "PlayerSH.h"
#include "ImageController.h"
#include "Dx12Ctrl.h"

const float GROUND_LINE = -100;

BackGround::BackGround(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<PlayerSH> spPlayer):IDrawableObject(imgCtrl),mGroundLine(GROUND_LINE),mwpPlayer(spPlayer)
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 wndSize = d12->GetWindowSize();
	DirectX::XMFLOAT2 imgSize = mImgCtrl->GetImageSize();
	mPos = { imgSize.x / 2.f - wndSize.x / 2.f, imgSize.y / 2.f - wndSize.y / 2.f, 0.f };
	mImgCtrl->SetPos(mPos);
	mSecondImage = mImgCtrl->GetNewCopy();
	mSecondImage->SetPos(mPos.x + imgSize.x, mPos.y, mPos.z);
	mSecondImage->TurnX();
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
	if (IsGroundPlayer())
	{
		mwpPlayer.lock()->OnGround(mGroundLine);
	}
}

void BackGround::Draw()
{
	mImgCtrl->Draw();
	mSecondImage->Draw();
}

bool BackGround::IsGroundPlayer() const
{
	if (mwpPlayer.lock()->GetPlayerPos().y <= mGroundLine)
	{
		return true;
	}
	return false;
}
