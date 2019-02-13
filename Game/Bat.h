#pragma once
#include "Enemy.h"
class Bat :
	public Enemy
{
public:
	Bat(std::shared_ptr<ImageController>& imcCtrl, std::shared_ptr<PlayerSH> spPlayer);
	Bat(std::shared_ptr<ImageController>& imgCtrl, float x, float y, float z, std::shared_ptr<PlayerSH> spPlayer);
	~Bat();

	void Draw() const;
	void Update();
	void OnGround(float groundLine);
	void OnDamage();
private:
	void (Bat::*mActionUpdate)();

	void Wait();
	void Damage();
	void Die();
	void Fly();
	void FlyDown();
};

