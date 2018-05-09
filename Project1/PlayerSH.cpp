#include "PlayerSH.h"
#include "ImageController.h"


PlayerSH::PlayerSH(ImageController* imgCtrl, std::shared_ptr<DxLibInput> dlibInput) :IDrawableObject::IDrawableObject(imgCtrl), mInput(dlibInput)
{
}

PlayerSH::~PlayerSH()
{
}

void PlayerSH::Update()
{

}

void PlayerSH::Draw()
{
	mImgCtrl->Draw();
}