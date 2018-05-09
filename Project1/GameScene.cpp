#include "GameScene.h"
#include "DxInput.h"
#include "PlayerSH.h"


GameScene::GameScene(std::shared_ptr<DxInput> inInput):mInput(inInput)
{
}


GameScene::~GameScene()
{
}

void GameScene::Run()
{
	mInput->UpdateKeyState();
	mPlayer->Update();
}

