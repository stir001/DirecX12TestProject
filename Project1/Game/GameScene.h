#pragma once
#include "Scene.h"

#include <memory>
#include <list>

class DxInput;
class ImageLoader;
class PlayerSH;
class BackGround;
class HeadUpDisplay;
class ActionLoader;
class Enemy;
class CollisionDetector;
class GameCamera2D;
class StageLoader;
class StageData;
class EnemyCreator;

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
	std::shared_ptr<BackGround> mBackGround;
	std::unique_ptr<HeadUpDisplay> mTopHUD;
	std::unique_ptr<HeadUpDisplay> mBottomHUD;
	std::shared_ptr<ActionLoader> mActLoader;
	std::shared_ptr<CollisionDetector> mColDetector;
	std::shared_ptr<GameCamera2D> mCamera2D;
	std::unique_ptr<StageLoader> mStageLoader;
	std::shared_ptr<StageData> mStageData;
	std::unique_ptr<EnemyCreator> mEnemyCreator;

	std::list<std::shared_ptr<Enemy>> mEnemys;

	void CreateHUD();
	void CreatePlayer();
	void CreateGround();
	void CreateCamera();
	void LoadStage();

	void LoadEnemyStageData();

	void CheckCollision();
};

