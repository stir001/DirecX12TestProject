#include "ICharactor.h"
#include "ActionStructs.h"
#include "XMFloat3Operators.h"
#include "ImageController.h"
#include <algorithm>

ICharactor::ICharactor(): mIsturn(false), mFrame(0), mActionImageIndex(0), mPos(0,0,0), mVel(0,0,0)
{
}

ICharactor::ICharactor(std::shared_ptr<ImageController>& imgCtrl):IDrawableObject(imgCtrl),  mIsturn(false), mFrame(0), mActionImageIndex(0), mPos(0, 0, 0), mVel(0, 0, 0)
{

}

ICharactor::~ICharactor()
{
}

void ICharactor::SetAction(ActionData& inActs)
{
	mActions = inActs.action;
	mCurrentAction = mActions.begin();
	mFrame = 0;
	mActionImageIndex = 0;
	SetActionImageData();
}

const DirectX::XMFLOAT3& ICharactor::GetPos() const
{
	return mPos;
}

void ICharactor::UpdatePostion()
{
	mPos += mVel;
	mImgCtrl->SetPos(mPos);
}

void ICharactor::AnimationUpdate()
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

void ICharactor::SetActionImageData()
{
	DirectX::XMFLOAT2 imgSize = { mCurrentAction->datas[mActionImageIndex].imageRect.GetWidth(),mCurrentAction->datas[mActionImageIndex].imageRect.GetHeight() };
	mImgCtrl->SetCenterOffset((mCurrentAction->datas[mActionImageIndex].pivot.x - imgSize.x / 2.0f), (-mCurrentAction->datas[mActionImageIndex].pivot.y + imgSize.y / 2.0f), 0);
	mImgCtrl->SetRect(mCurrentAction->datas[mActionImageIndex].imageRect);
}

void ICharactor::ChangeAction(const char* actionName)
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