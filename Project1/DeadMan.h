#pragma once
#include "Enemy.h"

class DeadMan :
	public Enemy
{
public:
	DeadMan(std::shared_ptr<ImageController>& imcCtrl);
	DeadMan(std::shared_ptr<ImageController>& imgCtrl, float x, float y, float z);
	~DeadMan();

	void Draw() const;
	void Update();
	void OnGround(float groundLine);
	void Gravity();
};

