#include "EnemyCreator.h"
#include "Enemy.h"
#include "ActionLoader.h"
#include "EnemyActionPathDefine.h"
#include "ImageController.h"
#include "ImageLoader.h"
#include "DeadMan.h"
#include "Bat.h"
#include "StageData.h"
#include "PlayerSH.h"

const int WINDOW_WIDTH = 768;

EnemyCreator::EnemyCreator(std::shared_ptr<ActionLoader> actLoader, std::shared_ptr<PlayerSH>& player) 
	:mActLoader(actLoader), mReadLine(0), mPlayer(player)
{
	mCreateFunc[eENEMY_TYPE_NON] = &EnemyCreator::CreateNullEnemy;
	mCreateFunc[eENEMY_TYPE_DEADMAN] = &EnemyCreator::CreateDeadMan;
	mCreateFunc[eENEMY_TYPE_BAT] = &EnemyCreator::CreateBat;
	mCreateFunc[eENEMY_TYPE_SPIKE] = &EnemyCreator::CreatePike;
}

EnemyCreator::~EnemyCreator()
{
}

void EnemyCreator::CreateEnemyStageData(std::list<std::shared_ptr<Enemy>>& enemys)
{
	while ((static_cast<int>(mPlayer->GetPos().x) + (WINDOW_WIDTH)) > (mReadLine * mData->GetChipSize().x) && mReadLine < mData->GetChipNum().x * mData->GetChipNum().y)
	{
		for (int i = 0; i < mData->GetChipNum().y; ++i)
		{
			int data = mData->GetChipData(0, mReadLine++, 0);
			if (data != 0)
			{
				(this->*mCreateFunc[data])(enemys);
			}
		}
	}
}

void EnemyCreator::SetStageData(std::shared_ptr<StageData>& data)
{
	mData = data;
}

void EnemyCreator::CreateNullEnemy(std::list<std::shared_ptr<Enemy>>& enemys)
{
}

void EnemyCreator::CreateDeadMan(std::list<std::shared_ptr<Enemy>>& enemys)
{
	auto& act = mActLoader->LoadActionData(DEADMAN_ACTION_PATH);
	std::shared_ptr<ImageController> ctrl = ImageLoader::Instance()->LoadImageData(act.relativePath);
	std::shared_ptr<Enemy> enemy(new DeadMan(ctrl, mPlayer));
	enemy->SetAction(act);
	enemys.push_back(enemy);
}

void EnemyCreator::CreateBat(std::list<std::shared_ptr<Enemy>>& enemys)
{
	auto& act = mActLoader->LoadActionData(BAT_ACTION_PATH);
	std::shared_ptr<ImageController> ctrl = ImageLoader::Instance()->LoadImageData(act.relativePath);
	std::shared_ptr<Enemy> enemy(new Bat(ctrl, mPlayer));
	enemy->SetAction(act);
	enemys.push_back(enemy);
}

void EnemyCreator::CreatePike(std::list<std::shared_ptr<Enemy>>& enemys)
{

}
