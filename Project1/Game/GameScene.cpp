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
#include "GameCamera2D.h"
#include "StageLoader.h"
#include "EnemyCreator.h"

const std::string BACKGROUND_IMAGE_PATH = "Action18/img/splatterhouse.png";
const std::string TOPHUD_IMAGE_PATH = "Action18/img/bar_top.png";
const std::string BOTTOMHUD_IMAGE_PATH = "Action18/img/bar_bottom.png";
const std::string PLAYER_ACTION_PATH = "Action18/Action/player.act";
const std::string DEADMAN_ACTION_PATH = "Action18/Action/deadman.act";
const std::string MAPDATA_PATH = "Action18/stagedata/stage1.fmf";

GameScene::GameScene(std::shared_ptr<DxInput> inInput):mInput(inInput)
, mPlayer(nullptr), mBackGround(nullptr), mTopHUD(nullptr), mBottomHUD(nullptr), mCamera2D(nullptr)
, mActLoader(new ActionLoader()), mColDetector(new CollisionDetector())
, mStageLoader(new StageLoader()), mEnemyCreator(nullptr)
{
	CreateHUD();
	CreatePlayer();
	CreateGround();
	CreateCamera();
	LoadStage();
}

GameScene::~GameScene()
{
	
}

void GameScene::Run()
{
	LoadEnemyStageData();

	mInput->UpdateKeyState();
	mBackGround->Update();
	mPlayer->Update();

	for (auto itr = mEnemys.begin();itr != mEnemys.end(); ++itr)
	{
		(*itr)->Update();
		if ((*itr)->IsDead())
		{
			mEnemys.erase(itr);
		}
	}

	CheckCollision();

	mCamera2D->Update();
	
	mCamera2D->DrawObjects();

	mBottomHUD->Draw();
	mTopHUD->Draw();
}

void GameScene::CreateHUD()
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 wndSize = d12->GetWindowSize();
	std::shared_ptr<ImageController> ictrl;
	DirectX::XMFLOAT2 imgSize;
	ictrl = (ImageLoader::Instance()->LoadImageData(BOTTOMHUD_IMAGE_PATH));
	imgSize = ictrl->GetImageSize();
	mBottomHUD.reset(new HeadUpDisplay(ictrl));
	mBottomHUD->SetPos(0, -(wndSize.y / 2.0f - imgSize.y / 2.0f), 0);

	ictrl = (ImageLoader::Instance()->LoadImageData(TOPHUD_IMAGE_PATH));
	imgSize = ictrl->GetImageSize();
	mTopHUD.reset(new HeadUpDisplay(ictrl));
	mTopHUD->SetPos(0, (wndSize.y / 2.0f - imgSize.y / 2.0f), 0);

}

void GameScene::CreatePlayer()
{
	ActionData& act = mActLoader->LoadActionData(PLAYER_ACTION_PATH);
	mPlayer.reset(new PlayerSH(ImageLoader::Instance()->LoadImageData(act.relativePath), mInput));
	mPlayer->SetAction(act);
}

void GameScene::CreateGround()
{
	std::shared_ptr<ImageController> imgCtrl = ImageLoader::Instance()->LoadImageData(BACKGROUND_IMAGE_PATH);
	mBackGround.reset(new BackGround(imgCtrl, mPlayer,mEnemys));
}

void GameScene::CreateCamera()
{
	mCamera2D.reset(new GameCamera2D(mPlayer,mBackGround, mEnemys));
}

void GameScene::LoadStage()
{
	mStageData = mStageLoader->LoadStageData(MAPDATA_PATH);
	mEnemyCreator.reset(new EnemyCreator(mActLoader, mPlayer));
	mEnemyCreator->SetStageData(mStageData);
}

void GameScene::LoadEnemyStageData()
{
	mEnemyCreator->CreateEnemyStageData(mEnemys);
}

void GameScene::CheckCollision()
{
	for (auto& e : mEnemys)
	{
		mColDetector->CheckCollision(mPlayer, e);
	}
}