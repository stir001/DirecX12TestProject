#pragma once
#include "Scene.h"
#include <memory>
class DxInput;
class ImageLoader;
class PlayerSH;
class BackGround;
class HeadUpDisplay;

class GameScene :
	public Scene
{
public:
	GameScene(std::shared_ptr<DxInput> inInput);
	~GameScene();
	void Run();

private:
	std::shared_ptr<DxInput> mInput;
	std::unique_ptr<ImageLoader> mImgLoader;
	std::shared_ptr<PlayerSH> mPlayer;
	std::unique_ptr<BackGround> mBackGround;
	std::unique_ptr<HeadUpDisplay> mTopHUD;
	std::unique_ptr<HeadUpDisplay> mBottomHUD;
};

