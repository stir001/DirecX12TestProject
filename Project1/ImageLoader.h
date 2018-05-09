#pragma once

#include <string>
#include <map>
#include <memory>

class ImageController;
class ImageObject;

class ImageLoader
{
private:
	std::map<std::string, std::shared_ptr<ImageObject>> images;
public:
	ImageLoader();
	ImageController* LoadImageData(std::string& path);
	~ImageLoader();
};

