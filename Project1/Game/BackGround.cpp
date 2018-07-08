#include "BackGround.h"
#include "PlayerSH.h"
#include "ImageController.h"
#include "Dx12Ctrl.h"
#include "ICharactor.h"
#include "XMFloatOperators.h"
#include "Enemy.h"

const float GROUND_LINE = -100;

BackGround::BackGround(std::shared_ptr<ImageController> imgCtrl, std::shared_ptr<ICharactor> spPlayer, std::list<std::shared_ptr<Enemy>>& enemy)
	:IDrawableObject(imgCtrl),mGroundLine(GROUND_LINE), mEnemys(enemy)
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 wndSize = d12->GetWindowSize();
	DirectX::XMFLOAT2 imgSize = mImgCtrl->GetImageSize();
	mPos = { imgSize.x / 2.f - wndSize.x / 2.f, imgSize.y / 2.f - wndSize.y / 2.f, 0.f };
	mImgCtrl->SetPos(mPos);
	mSecondImage = mImgCtrl->GetNewCopy();
	mSecondPos = { mPos.x + imgSize.x, mPos.y, mPos.z };
	mSecondImage->SetPos(mSecondPos);
	mSecondImage->TurnX();
	mThirdPos = { mPos.x + imgSize.x * 2, mPos.y, mPos.z };
	mThirdImage = mImgCtrl->GetNewCopy();
	mThirdImage->SetPos(mThirdPos);
	mCharactor.push_back(spPlayer);
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
	for (auto& c : mCharactor)
	{
		if (IsGroundCharactor(c))
		{
			c->OnGround(mGroundLine);
		}
	}

	for (auto& e : mEnemys)
	{
		if (IsGroundCharactor(e))
		{
			e->OnGround(mGroundLine);
		}
	}
}

void BackGround::Draw() const
{
	mImgCtrl->Draw();
	mSecondImage->Draw();
	mThirdImage->Draw();
}

void BackGround::Draw(const DirectX::XMFLOAT3& offset)
{
	int offx = static_cast<int>(offset.x - mImgCtrl->GetImageSize().x * 2);
	int ImgSize3Rate = static_cast<int>(mImgCtrl->GetImageSize().x * 3);
	mImgCtrl->SetPos(mPos.x + mImgCtrl->GetImageSize().x * 2 + offx % ImgSize3Rate, mPos.y, mPos.z);
	if (!mImgCtrl->IsTurnX() && (-offx / ImgSize3Rate) % 2)
	{
		mImgCtrl->TurnX();
	}
	else if(mImgCtrl->IsTurnX() && !((-offx / ImgSize3Rate) % 2))
	{
		mImgCtrl->TurnX();
	}

	offx = static_cast<int>(offset.x - mSecondImage->GetImageSize().x);
	mSecondImage->SetPos(mSecondPos.x + mSecondImage->GetImageSize().x  + (offx % ImgSize3Rate), mSecondPos.y, mSecondPos.z);
	if (mSecondImage->IsTurnX() && (-offx / ImgSize3Rate) % 2)
	{
		mSecondImage->TurnX();
	}
	else if (!mSecondImage->IsTurnX() && !((-offx / ImgSize3Rate) % 2))
	{
		mSecondImage->TurnX();
	}

	mThirdImage->SetPos(mThirdPos.x  + (static_cast<int>(offset.x ) % ImgSize3Rate), mThirdPos.y, mThirdPos.z);
	if (!mThirdImage->IsTurnX() && (-static_cast<int>(offset.x) / ImgSize3Rate) % 2)
	{
		mThirdImage->TurnX();
	}
	else if (mThirdImage->IsTurnX() && !((-static_cast<int>(offset.x) / ImgSize3Rate) % 2))
	{
		mThirdImage->TurnX();
	}

	Draw();
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
	mCharactor.push_back(charactor);
}