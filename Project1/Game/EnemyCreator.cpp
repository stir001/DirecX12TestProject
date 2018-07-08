#include "EnemyCreator.h"
#include "Enemy.h"
#include "DeadMan.h"
#include "ActionLoader.h"
#include "EnemyActionPathDefine.h"
#include "ImageController.h"
#include "ImageLoader.h"

EnemyCreator::EnemyCreator(std::shared_ptr<ActionLoader> actLoader)
{
	mCreateFunc[eENEMY_TYPE_NON] = &EnemyCreator::CreateNullEnemy;
	mCreateFunc[eENEMY_TYPE_DEADMAN] = &EnemyCreator::CreateDeadMan;
	mCreateFunc[eENEMY_TYPE_BAD] = &EnemyCreator::CreateBad;
	mCreateFunc[eENEMY_TYPE_SPIKE] = &EnemyCreator::CreateSpike;
}

EnemyCreator::~EnemyCreator()
{
}

std::shared_ptr<Enemy> EnemyCreator::CreateEnemy(unsigned int enemyNum)
{
	return (this->*mCreateFunc[enemyNum])();
}

std::shared_ptr<Enemy> EnemyCreator::CreateNullEnemy()
{
	return nullptr;
}

std::shared_ptr<Enemy> EnemyCreator::CreateDeadMan()
{
	auto& act = mActLoader->LoadActionData(DEADMAN_ACTION_PATH);
	std::shared_ptr<ImageController> ctrl = ImageLoader::Instance()->LoadImageData(act.relativePath);
	std::shared_ptr<Enemy> enemy(new DeadMan(ctrl, mPlayer));
	enemy->SetAction(act);
	return enemy;
}

std::shared_ptr<Enemy> EnemyCreator::CreateBad()
{
	return nullptr;
}

std::shared_ptr<Enemy> EnemyCreator::CreateSpike()
{
	return nullptr;
}
