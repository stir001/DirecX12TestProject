#include "BackGround.h"
#include "PlayerSH.h"
#include "ImageController.h"
#include "Dx12Ctrl.h"
#include "ICharactor.h"

const float GROUND_LINE = -100;

BackGround::BackGround(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<ICharactor> spPlayer):IDrawableObject(imgCtrl),mGroundLine(GROUND_LINE)
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 wndSize = d12->GetWindowSize();
	DirectX::XMFLOAT2 imgSize = mImgCtrl->GetImageSize();
	mPos = { imgSize.x / 2.f - wndSize.x / 2.f, imgSize.y / 2.f - wndSize.y / 2.f, 0.f };
	mImgCtrl->SetPos(mPos);
	mSecondImage = mImgCtrl->GetNewCopy();
	mSecondImage->SetPos(mPos.x + imgSize.x, mPos.y, mPos.z);
	mSecondImage->TurnX();
	mspCharactor.push_back(spPlayer);
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
	for (auto& c : mspCharactor)
	{
		if (IsGroundCharactor(c))
		{
			c->OnGround(mGroundLine);
		}
	}
}

void BackGround::Draw() const
{
	mImgCtrl->Draw();
	mSecondImage->Draw();
}

bool BackGround::IsGroundCharactor(std::shared_ptr<ICharactor> charactor) const
{
	if (charactor->GetPos().y <= mGroundLine)
	{
		return true;
	}
	return false;
}

void BackGround::SetCharactor(std::shared_ptr<ICharactor> charactor)
{
	mspCharactor.push_back(charactor);
}