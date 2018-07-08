#pragma once
#include <DirectXMath.h>
#include <vector>

class  StageData
{
public:
	StageData(const std::vector<int>& mapData, const DirectX::XMINT2& chipNum, const DirectX::XMINT2& chipSize);
	~StageData();

	int GetChipData(int layer, int x, int y);
private:


	DirectX::XMINT2 mChipNum;
	DirectX::XMINT2 mChipSize;
	std::vector<int> mMapData;
};

