#include "IDrawableObject.h"
#include "ImageController.h"


IDrawableObject::IDrawableObject()
{
}

IDrawableObject::IDrawableObject(ImageController* imgCtrl):mImgCtrl(imgCtrl)
{

}


IDrawableObject::~IDrawableObject()
{
	delete mImgCtrl;
}

void IDrawableObject::SetImageController(ImageController* imgCtrl)
{
	if (mImgCtrl == nullptr)
	{
		delete mImgCtrl;
	}
	mImgCtrl = imgCtrl;
}

void IDrawableObject::Update()
{
}
