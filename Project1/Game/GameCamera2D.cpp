#include "GameCamera2D.h"
#include "IDrawableObject.h"
#include "XMFloatOperators.h"	
#include "PlayerSH.h"
#include "BackGround.h"

#include <algorithm>


GameCamera2D::GameCamera2D(std::shared_ptr<PlayerSH>& player, std::shared_ptr<BackGround>& backGround)
	: mObjectsNum(0), mPlayer(player), mCameraPos{0,0,0}, mStateImageNum(6), mBackGround(backGround)
{
}

GameCamera2D::~GameCamera2D()
{
}

void GameCamera2D::Update()
{
	float max = std::max(mPlayer->GetPos().x, 0.0f);
	float min = std::min(max, static_cast<float>(STAGE_IMAGE_SIZE_X * (mStateImageNum - 1)));
	mCameraPos.x = min;
}

void GameCamera2D::DrawObjects()
{
	mBackGround->Draw(-mCameraPos);
	for (auto& obj : mObjects)
	{
		obj.second.lock()->Draw(-mCameraPos);
	}
	mPlayer->Draw(-mCameraPos);
}

int GameCamera2D::SetObject(std::shared_ptr<IDrawableObject> drawobject)
{
	mObjects[mObjectsNum++] = drawobject;
	return mObjectsNum - 1;
}

void GameCamera2D::RemoveDrawObject(int objectNum)
{
	auto itr = mObjects.find(objectNum);
	if (itr == mObjects.end())
	{
		mObjects.erase(itr);
	}
}
