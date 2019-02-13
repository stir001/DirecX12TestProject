#pragma once
#include <memory>
#include <list>

class ActionLoader;
class PlayerSH;
class Enemy;
class StageData;

class EnemyCreator
{
public:
	EnemyCreator(std::shared_ptr<ActionLoader> actLoader, std::shared_ptr<PlayerSH>& player);
	~EnemyCreator();

	void CreateEnemyStageData(std::list<std::shared_ptr<Enemy>>& enemys);
	void SetStageData(std::shared_ptr<StageData>& data);
private:
	enum eENEMY_TYPE {
		eENEMY_TYPE_NON,
		eENEMY_TYPE_DEADMAN,
		eENEMY_TYPE_BAT,
		eENEMY_TYPE_SPIKE,
		eENEMY_TYPE_MAX,
	};
	void (EnemyCreator::*mCreateFunc[eENEMY_TYPE_MAX])(std::list<std::shared_ptr<Enemy>>& enemys,int x, int y);
	std::shared_ptr<ActionLoader> mActLoader;
	std::shared_ptr<PlayerSH> mPlayer;
	std::shared_ptr<StageData> mData;

	void CreateNullEnemy(std::list<std::shared_ptr<Enemy>>& enemys, int x, int y);
	void CreateDeadMan(std::list<std::shared_ptr<Enemy>>& enemys, int x, int y);
	void CreateBat(std::list<std::shared_ptr<Enemy>>& enemys, int x, int y);
	void CreatePike(std::list<std::shared_ptr<Enemy>>& enemys, int x, int y);

	int mReadLine;
};

