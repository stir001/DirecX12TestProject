#include "Enemy.h"



Enemy::Enemy(std::shared_ptr<ImageController>& imgCtrl, std::shared_ptr<PlayerSH> spPlayer):ICharactor(imgCtrl),mwpPlayer(spPlayer)
{
}


Enemy::~Enemy()
{
}
