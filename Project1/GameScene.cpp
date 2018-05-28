#include "GameScene.h"
#include "DxInput.h"
#include "PlayerSH.h"
#include "ImageLoader.h"
#include "BackGround.h"
#include "HeadUpDisplay.h"
#include "ImageController.h"
#include "ActionLoader.h"
#include "Dx12Ctrl.h"
#include "DeadMan.h"
#include "CollisionDetector.h"

const std::string BACKGROUND_IMAGE_PATH = "Action18/img/splatterhouse.png";
const std::string TOPHUD_IMAGE_PATH = "Action18/img/bar_top.png";
const std::string BOTTOMHUD_IMAGE_PATH = "Action18/img/bar_bottom.png";
const std::string PLAYER_ACTION_PATH = "Action18/Action/player.act";
const std::string DEADMAN_ACTION_PATH = "Action18/Action/deadman.act";

GameScene::GameScene(std::shared_ptr<DxInput> inInput):mInput(inInput),mImgLoader(new ImageLoader())
,mPlayer(nullptr), mBackGround(nullptr), mTopHUD(nullptr), mBottomHUD(nullptr)
,mActLoader(new ActionLoader()),mColDetector(new CollisionDetector())
{
	CreateHUD();
	CreatePlayer();
	CreateEnemy(100,0,0);
	CreateEnemy(-100,0,0);
	CreateGround();
}


GameScene::~GameScene()
{
	
}

void GameScene::Run()
{
	mInput->UpdateKeyState();
	mBackGround->Update();
	mPlayer->Update();

	for (auto& e : mEnemeys)
	{
		e->Update();
	}

	CheckCollision();

	mTopHUD->Draw();
	mBottomHUD->Draw();
	mPlayer->Draw();
	for (auto& e : mEnemeys)
	{
		e->Draw();
	}
	mBackGround->Draw();
}

void GameScene::CreateHUD()
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 wndSize = d12->GetWindowSize();
	std::shared_ptr<ImageController> ictrl;
	DirectX::XMFLOAT2 imgSize;
	ictrl = (mImgLoader->LoadImageData(BOTTOMHUD_IMAGE_PATH));
	imgSize = ictrl->GetImageSize();
	mBottomHUD.reset(new HeadUpDisplay(ictrl));
	mBottomHUD->SetPos(0, -(wndSize.y / 2.0f - imgSize.y / 2.0f), 0);

	ictrl = (mImgLoader->LoadImageData(TOPHUD_IMAGE_PATH));
	imgSize = ictrl->GetImageSize();
	mTopHUD.reset(new HeadUpDisplay(ictrl));
	mTopHUD->SetPos(0, (wndSize.y / 2.0f - imgSize.y / 2.0f), 0);

}

void GameScene::CreatePlayer()
{
	ActionData& act = mActLoader->LoadActionData(PLAYER_ACTION_PATH);
	mPlayer.reset(new PlayerSH(mImgLoader->LoadImageData(act.relativePath), mInput));
	mPlayer->SetAction(act);
}

void GameScene::CreateBackGround()
{
	mBackGround.reset(new BackGround(mImgLoader->LoadImageData(BACKGROUND_IMAGE_PATH), mPlayer));
}

void GameScene::CreateEnemy(float x, float y, float z)
{
	ActionData& act = mActLoader->LoadActionData(DEADMAN_ACTION_PATH);
	std::shared_ptr<ImageController> imgCtrl = mImgLoader->LoadImageData(act.relativePath);
	std::shared_ptr<Enemy> enemy(new DeadMan(imgCtrl, x, y, z, mPlayer));
	enemy->SetAction(act);
	mEnemeys.push_back(enemy);
}

void GameScene::CreateGround()
{
	std::shared_ptr<ImageController> imgCtrl = mImgLoader->LoadImageData(BACKGROUND_IMAGE_PATH);
	mBackGround.reset(new BackGround(imgCtrl, mPlayer));
	for (auto& enemy : mEnemeys)
	{
		mBackGround->SetCharactor(enemy);
	}
}

void GameScene::CheckCollision()
{
	for (auto& e : mEnemeys)
	{
		mColDetector->CheckCollision(mPlayer, e);
	}
}