#include "GameScene.h"
#include "DxInput.h"
#include "PlayerSH.h"
#include "ImageLoader.h"
#include "BackGround.h"
#include "HeadUpDisplay.h"
#include "ImageController.h"
#include "ActionLoader.h"
#include "Dx12Ctrl.h"

const std::string PLAYER_IMAGE_PATH = "Action18/img/rick.png";
const std::string BACKGROUND_IMAGE_PATH = "Action18/img/splatterhouse.png";
const std::string TOPHUD_IMAGE_PATH = "Action18/img/bar_top.png";
const std::string BOTTOMHUD_IMAGE_PATH = "Action18/img/bar_bottom.png";
const std::string PLAYER_ACTION_PATH = "Action18/Action/player.act";

GameScene::GameScene(std::shared_ptr<DxInput> inInput):mInput(inInput),mImgLoader(new ImageLoader())
,mPlayer(new PlayerSH(mImgLoader->LoadImageData(PLAYER_IMAGE_PATH), inInput))
,mBackGround(new BackGround(mImgLoader->LoadImageData(BACKGROUND_IMAGE_PATH),mPlayer))
,mActLoader(new ActionLoader())
{
	LoadHUD();
	mPlayer->SetAction(mActLoader->LoadActionData(PLAYER_ACTION_PATH));
}


GameScene::~GameScene()
{
	
}

void GameScene::Run()
{
	mInput->UpdateKeyState();
	mBackGround->Update();
	mPlayer->Update();

	//mTopHUD->Draw();
	mBottomHUD->Draw();
	mPlayer->Draw();
	mBackGround->Draw();
}

void GameScene::LoadHUD()
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