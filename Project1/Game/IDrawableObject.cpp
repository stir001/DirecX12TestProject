#include "IDrawableObject.h"
#include "ImageController.h"


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
