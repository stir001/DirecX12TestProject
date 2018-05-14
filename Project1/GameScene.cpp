#include "GameScene.h"
#include "DxInput.h"
#include "PlayerSH.h"
#include "ImageLoader.h"
#include "BackGround.h"
#include "HeadUpDisplay.h"
#include "ImageController.h"
#include <dxgiformat.h>

const std::string PLAYER_IMAGE_PATH = "Action18/img/rick.png";
const std::string BACKGROUND_IMAGE_PATH = "Action18/img/splatterhouse.png";
const std::string TOPHUD_IMAGE_PATH = "Action18/img/bar_top.png";
const std::string BOTTOMHUD_IMAGE_PATH = "Action18/img/bar_bottom.png";

GameScene::GameScene(std::shared_ptr<DxInput> inInput):mInput(inInput),mImgLoader(new ImageLoader())
{
	mPlayer.reset(new PlayerSH(mImgLoader->LoadImageData(PLAYER_IMAGE_PATH), mInput));
	mBackGround.reset(new BackGround(mImgLoader->LoadImageData(BACKGROUND_IMAGE_PATH),mPlayer));
	std::shared_ptr<ImageController> ictrl(mImgLoader->LoadImageData(TOPHUD_IMAGE_PATH));
	DirectX::XMFLOAT2 size = ictrl->GetImageSize();
	mTopHUD.reset(new HeadUpDisplay(ictrl));
	//mTopHUD->SetPos();
	mBottomHUD.reset(new HeadUpDisplay(mImgLoader->LoadImageData(BOTTOMHUD_IMAGE_PATH)));
}


GameScene::~GameScene()
{
	
}

void GameScene::Run()
{
	mInput->UpdateKeyState();
	mPlayer->Update();

	mTopHUD->Draw();
	//mBottomHUD->Draw();
	/*mPlayer->Draw();
	mBackGround->Draw();*/
}

