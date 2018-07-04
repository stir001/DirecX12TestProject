#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
class DxInput;
class ImageLoader;
class PlayerSH;
class BackGround;
class HeadUpDisplay;
class ActionLoader;
class Enemy;
class CollisionDetector;

class GameScene :
	public Scene
{
public:
	GameScene(std::shared_ptr<DxInput> inInput);
	~GameScene();
	void Run();

private:
	std::shared_ptr<DxInput> mInput;
	std::shared_ptr<PlayerSH> mPlayer;
	std::unique_ptr<BackGround> mBackGround;
	std::unique_ptr<HeadUpDisplay> mTopHUD;
	std::unique_ptr<HeadUpDisplay> mBottomHUD;
	std::unique_ptr<ActionLoader> mActLoader;
	std::shared_ptr<CollisionDetector> mColDetector;

	std::vector<std::shared_ptr<Enemy>> mEnemeys;

	void CreateHUD();
	void CreatePlayer();
	void CreateBackGround();
	void CreateEnemy(float x, float y,float z);
	void CreateGround();

	void CheckCollision();
};
