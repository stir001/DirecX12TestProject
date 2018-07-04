#include "ICharactor.h"
#include "ActionStructs.h"
#include "XMFloatOperators.h"
#include "ImageController.h"
#include "Primitive2DLine.h"
#include "Primitive2DManager.h"
#include <algorithm>

ICharactor::ICharactor(): mIsturn(false), mFrame(0), mActionImageIndex(0), mPos(0,0,0), mVel(0,0,0)
{
	mImgCtrl->SetScale(CHARACTOR_SCALE);
}

ICharactor::ICharactor(std::shared_ptr<ImageController>& imgCtrl):IDrawableObject(imgCtrl),  mIsturn(false), mFrame(0), mActionImageIndex(0), mPos(0, 0, 0), mVel(0, 0, 0)
{
	mImgCtrl->SetScale(CHARACTOR_SCALE);
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
#ifdef _DEBUG
	UpDateRectPosition();
#endif // _DEBUG
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

#ifdef _DEBUG
	UpDateRectLine();
#endif // _DEBUG
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

const std::vector<ActionRect>& ICharactor::GetCurrentActionRects() const
{
	return mCurrentAction->datas[mActionImageIndex].actionRects;
}

bool ICharactor::IsTurn() const
{
	return mIsturn;
}

#ifdef _DEBUG
void ICharactor::DrawRect() const
{
	for (auto& line : mRectLines)
	{
		line->Draw();
	}
}

void ICharactor::UpDateRectLine()
{
	DirectX::XMFLOAT3 color;
	mRectLines.clear();
	mRectLines.reserve(mCurrentAction->datas[mActionImageIndex].actionRects.size() * 4);
	DirectX::XMFLOAT3 offset = {-(mCurrentAction->datas[mActionImageIndex].pivot.x), -(-mCurrentAction->datas[mActionImageIndex].pivot.y), 0};
	offset += mPos;
	for (auto& actRect : mCurrentAction->datas[mActionImageIndex].actionRects)
	{
		if (actRect.type == eACTION_RECT_TYPE_ATTACK)
		{
			color = { 1,0,0 };
		}
		else if (actRect.type == eACTION_RECT_TYPE_DAMAGE)
		{
			color = { 0,1,0 };
		}
		else
		{
			color = { 1,1,1 };
		}
		//上のライン
		mRectLines.push_back(Primitive2DManager::Instance()->CreatePrimitive2DLine(DirectX::XMFLOAT3(actRect.rect.GetLeft() + offset.x, actRect.rect.GetUp() + offset.y, 0),
			DirectX::XMFLOAT3(actRect.rect.GetRight() + offset.x, actRect.rect.GetUp() + offset.y, 0)));
		mRectLines.back()->SetColor(color);

		//右のライン
		mRectLines.push_back(Primitive2DManager::Instance()->CreatePrimitive2DLine(DirectX::XMFLOAT3(actRect.rect.GetRight() + offset.x, actRect.rect.GetUp() + offset.y, 0),
				DirectX::XMFLOAT3(actRect.rect.GetRight() + offset.x, actRect.rect.GetDown() + offset.y, 0)));
		mRectLines.back()->SetColor(color);

		//下のライン
		mRectLines.push_back(Primitive2DManager::Instance()->CreatePrimitive2DLine(DirectX::XMFLOAT3(actRect.rect.GetRight() + offset.x, actRect.rect.GetDown() + offset.y, 0),
				DirectX::XMFLOAT3(actRect.rect.GetLeft() + offset.x, actRect.rect.GetDown() + offset.y, 0)));
		mRectLines.back()->SetColor(color);

		//左のライン
		mRectLines.push_back(Primitive2DManager::Instance()->CreatePrimitive2DLine(DirectX::XMFLOAT3(actRect.rect.GetLeft() + offset.x, actRect.rect.GetDown() + offset.y, 0),
				DirectX::XMFLOAT3(actRect.rect.GetLeft() + offset.x, actRect.rect.GetUp() + offset.y, 0)));
		mRectLines.back()->SetColor(color);
	}
}

void ICharactor::UpDateRectPosition()
{
	DirectX::XMFLOAT3 offset = { -(mCurrentAction->datas[mActionImageIndex].pivot.x), -(-mCurrentAction->datas[mActionImageIndex].pivot.y), 0 };
	offset += mPos;
	DirectX::XMFLOAT3 setPos;
	int idx = 0;
	for (int i = 0; i < mRectLines.size();++idx)
	{
		setPos = { offset.x , mCurrentAction->datas[mActionImageIndex].actionRects[idx].rect.GetUp() + offset.y, 0 };
		mRectLines[i++]->SetCenter(setPos);

		setPos = { offset.x + mCurrentAction->datas[mActionImageIndex].actionRects[idx].rect.GetRight(), offset.y, 0 };
		mRectLines[i++]->SetCenter(setPos);

		setPos = { offset.x, offset.y - mCurrentAction->datas[mActionImageIndex].actionRects[idx].rect.GetDown(), 0 };
		mRectLines[i++]->SetCenter(setPos);

		setPos = { offset.x - mCurrentAction->datas[mActionImageIndex].actionRects[idx].rect.GetLeft(), offset.y, 0 };
		mRectLines[i++]->SetCenter(setPos);
	}
}
#endif // _DEBUG