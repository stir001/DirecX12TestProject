#pragma once
#include "Scene.h"
#include <memory>
class DxInput;
class ImageLoader;
class PlayerSH;

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
};

