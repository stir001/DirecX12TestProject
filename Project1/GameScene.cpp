#include "GameScene.h"
#include "DxInput.h"
#include "PlayerSH.h"
#include "ImageLoader.h"

const std::string PLAYER_IMAGE_PATH = "Action18/img/rick.png";

GameScene::GameScene(std::shared_ptr<DxInput> inInput):mInput(inInput),mImgLoader(new ImageLoader())
{
	
	mPlayer.reset(new PlayerSH(mImgLoader->LoadImageData(PLAYER_IMAGE_PATH), mInput));
}


GameScene::~GameScene()
{
}

void GameScene::Run()
{
	mInput->UpdateKeyState();
	mPlayer->Update();

	mPlayer->Draw();
}

