#include "HeadUpDisplay.h"
#include "ImageController.h"


HeadUpDisplay::HeadUpDisplay(std::shared_ptr<ImageController> imgCtrl):IDrawableObject(imgCtrl),mPos(0,0,0)
{
}


HeadUpDisplay::~HeadUpDisplay()
{
}

void HeadUpDisplay::SetPos(DirectX::XMFLOAT3& pos)
{
	mPos = pos;
	mImgCtrl->SetPos(pos);
}

void  HeadUpDisplay::Draw()
{
	mImgCtrl->Draw();
}
