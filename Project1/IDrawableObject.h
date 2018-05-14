#pragma once
#include <memory>

class ImageController;
class IDrawableObject
{
protected:
	std::shared_ptr<ImageController> mImgCtrl;
public:
	void SetImageController(std::shared_ptr<ImageController> imgCtrl);

	virtual void Draw() = 0;
	IDrawableObject();
	IDrawableObject(std::shared_ptr<ImageController> imgCtrl);
	virtual ~IDrawableObject();
};

