#pragma once

#include<string>
class ImageObject;

class ImageLoader
{
private:
public:
	ImageObject* LoadImageData(std::string& path);
	ImageLoader();
	~ImageLoader();
};

