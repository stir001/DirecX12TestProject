#pragma once
#include <DirectXMath.h>
#include <map>
#include <memory>
#include <list>

class PlayerSH;
class BackGround;
class Enemy;

class GameCamera2D
{
public:
	GameCamera2D(std::shared_ptr<PlayerSH>& player, std::shared_ptr<BackGround>& backGround, std::list<std::shared_ptr<Enemy>>& enemys);
	~GameCamera2D();

	void Update();
	void DrawObjects();
	void SetStageLength(int length);
private:
	const int WINDOW_SIZE_X = 768;
	const int STAGE_IMAGE_SIZE_X = 576;
	int mObjectsNum;

	int mStageLength;
	DirectX::XMFLOAT3 mCameraPos;
	std::shared_ptr<PlayerSH> mPlayer;
	std::shared_ptr<BackGround> mBackGround;
	std::list<std::shared_ptr<Enemy>>& mEnemys;
};

