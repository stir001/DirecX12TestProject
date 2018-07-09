#include "StageData.h"



StageData::StageData(const std::vector<int>& mapData, const DirectX::XMINT2& chipNum, const DirectX::XMINT2& chipSize)
	:mMapData(mapData), mChipNum(chipNum), mChipSize(chipSize)
{
}

StageData::~StageData()
{
}

int StageData::GetChipData(int layer, int x, int y)
{
	if (layer * mChipNum.x * mChipNum.y + x + y * mChipNum.x >= mChipNum.x * mChipNum.y)
	{
		return 0;
	}
	return mMapData[layer * mChipNum.x * mChipNum.y + x + y * mChipNum.x];
}

DirectX::XMINT2 StageData::GetChipSize() const
{
	return mChipSize;
}

DirectX::XMINT2 StageData::GetChipNum() const
{
	return mChipNum;
}
