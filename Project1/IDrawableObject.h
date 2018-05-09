#pragma once
class ImageController;
class IDrawableObject
{
protected:
	ImageController* mImgCtrl;
public:
	void SetImageController(ImageController* imgCtrl);

	virtual void Update();
	virtual void Draw() = 0;
	IDrawableObject();
	IDrawableObject(ImageController* imgCtrl);
	virtual ~IDrawableObject();
};

