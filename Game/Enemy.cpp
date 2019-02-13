#include "Enemy.h"



Enemy::Enemy(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<PlayerSH> spPlayer)
	:ICharactor(imgCtrl), mwpPlayer(spPlayer), mIsDead(false)
{
}


Enemy::~Enemy()
{
}

void Enemy::SetID(int id)
{
	mID = id;
}

int Enemy::GetID() const
{
	return mID;
}

bool Enemy::IsDead() const
{
	return mIsDead;
}
