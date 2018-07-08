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
	return mMapData[layer * mChipNum.x * mChipNum.y + x + y * mChipNum.x];
}
