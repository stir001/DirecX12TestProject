#pragma once
#include <memory>

class ImageController;

class IDrawableObject
{
public:
	IDrawableObject();
	IDrawableObject(std::shared_ptr<ImageController>& imgCtrl);
	virtual ~IDrawableObject();

	void SetImageController(std::shared_ptr<ImageController>& imgCtrl);

	virtual void Draw() const = 0;
protected:
	std::shared_ptr<ImageController> mImgCtrl;
};

