#include "GameCamera2D.h"
#include "IDrawableObject.h"
#include "XMFloatOperators.h"	
#include "PlayerSH.h"
#include "BackGround.h"
#include "Enemy.h"

#include <algorithm>


GameCamera2D::GameCamera2D(std::shared_ptr<PlayerSH>& player, std::shared_ptr<BackGround>& backGround, std::list<std::shared_ptr<Enemy>>& enemys)
	: mObjectsNum(0), mPlayer(player), mCameraPos{0,0,0},
	 mBackGround(backGround), mEnemys(enemys), mStageLength(STAGE_IMAGE_SIZE_X * 5)
{
}

GameCamera2D::~GameCamera2D()
{
}

void GameCamera2D::Update()
{
	float max = std::max(mPlayer->GetPos().x, 0.0f);
	float min = std::min(max, static_cast<float>(mStageLength - WINDOW_SIZE_X / 2));
	mCameraPos.x = min;
}

void GameCamera2D::DrawObjects()
{
	mBackGround->Draw(-mCameraPos);
	for (auto& e : mEnemys)
	{
		e->Draw(-mCameraPos);
	}
	mPlayer->Draw(-mCameraPos);
}

void GameCamera2D::SetStageLength(int length)
{
	mStageLength = length;
}
