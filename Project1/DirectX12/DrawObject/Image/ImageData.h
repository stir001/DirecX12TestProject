#pragma once

#include <vector>
#include <iostream>

class ImageData
{
private:
public:
	std::vector<char> rgbData;
	unsigned int width;
	unsigned int height;
	ImageData();
	~ImageData();
};

