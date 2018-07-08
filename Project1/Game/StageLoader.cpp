#include "StageLoader.h"
#include "StageData.h"
#include "File.h"


StageLoader::StageLoader()
{
	
}


StageLoader::~StageLoader()
{
}

const std::shared_ptr<StageData>& StageLoader::LoadStageData(const std::string& path)
{
	if (mFile == nullptr)
	{
		mFile.reset(new File(path));
	}
	else
	{
		mFile->Close();
		mFile->SetFile(path);
	}
	auto itr = mData.find(path);
	if (itr != mData.end())
	{
		return itr->second;
	}

	auto& data = LoadFMF();

	DirectX::XMINT2 mapNum = { mHeader.width, mHeader.height };
	DirectX::XMINT2 chipSize = { mHeader.chipWidth, mHeader.chipHeight };
	std::shared_ptr<StageData> rtn(new StageData(data, mapNum, chipSize));
}

std::vector<int> StageLoader::LoadFMF()
{
	mFile->LoadFile(&mHeader);
	size_t elementCount = mHeader.height * mHeader.width * mHeader.layerCount;
	std::vector<int> data(elementCount);
	std::vector<int> convertData(elementCount);
	for (auto& d : data)
	{
		mFile->LoadFile(&d, 1, mHeader.bitCount / 8);
	}

	for (int layer = 0; layer < mHeader.layerCount; ++layer)
	{
		for (int h = 0; h < mHeader.height; ++h)
		{
			for (int w = 0; w < mHeader.width; ++w)
			{

				convertData[layer * mHeader.width * mHeader.height + w + h * mHeader.width] =
					data[layer * mHeader.width * mHeader.height + w * mHeader.width + h ];

			}
		}
	}
}
