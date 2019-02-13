#include "IDrawableObject.h"
#include "Dx12MSLib.h"


IDrawableObject::IDrawableObject()
{
}

IDrawableObject::IDrawableObject(std::shared_ptr<ImageController>& imgCtrl):mImgCtrl(imgCtrl)
{

}


IDrawableObject::~IDrawableObject()
{
}

void IDrawableObject::SetImageController(std::shared_ptr<ImageController>& imgCtrl)
{
	mImgCtrl = imgCtrl;
}

void IDrawableObject::Draw(const DirectX::XMFLOAT3 & offset)
{
}
