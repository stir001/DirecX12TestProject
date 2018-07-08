#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

class File;
class StageData;

class StageLoader
{
public:
	StageLoader();
	~StageLoader();
	const std::shared_ptr<StageData>& LoadStageData(const std::string& path);
private:
	struct FMFHeader
	{
		char identifier[4];
		unsigned int size;
		unsigned int width;
		unsigned int height;
		unsigned char chipWidth;
		unsigned char chipHeight;
		unsigned char layerCount;
		unsigned char bitCount;
	};
	FMFHeader mHeader;


	std::shared_ptr<File> mFile;
	std::map <std::string, std::shared_ptr<StageData>> mData;

	std::vector<int> LoadFMF();
};

