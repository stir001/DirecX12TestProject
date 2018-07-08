#pragma once
#include <memory>

class ActionLoader;
class PlayerSH;
class Enemy;

class EnemyCreator
{
public:
	EnemyCreator(std::shared_ptr<ActionLoader> actLoader);
	~EnemyCreator();

	std::shared_ptr<Enemy> CreateEnemy(unsigned int enemyNum);
private:
	enum eENEMY_TYPE {
		eENEMY_TYPE_NON,
		eENEMY_TYPE_DEADMAN,
		eENEMY_TYPE_BAD,
		eENEMY_TYPE_SPIKE,
		eENEMY_TYPE_MAX,
	};
	std::shared_ptr<Enemy> (EnemyCreator::*mCreateFunc[eENEMY_TYPE_MAX])();
	std::shared_ptr<ActionLoader> mActLoader;
	std::shared_ptr<PlayerSH> mPlayer;

	std::shared_ptr<Enemy> CreateNullEnemy();
	std::shared_ptr<Enemy> CreateDeadMan();
	std::shared_ptr<Enemy> CreateBad();
	std::shared_ptr<Enemy> CreateSpike();
};

