#pragma once
#include "Enemy.h"

class DeadMan :
	public Enemy
{
public:
	DeadMan(std::shared_ptr<ImageController>& imcCtrl,std::shared_ptr<PlayerSH> spPlayer);
	DeadMan(std::shared_ptr<ImageController>& imgCtrl, float x, float y, float z, std::shared_ptr<PlayerSH> spPlayer);
	~DeadMan();

	void Draw() const;
	void Update();
	void OnGround(float groundLine);
	void OnDamage();

private:
	void (DeadMan::*mActionUpdate)();

	void Gravity();
	void Walk();
	void Damage();
	void Die();
	void Die2();

	int mDeadCount;
};

