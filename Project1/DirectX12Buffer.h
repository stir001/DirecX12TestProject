#pragma once
#include <d3d12.h>
#include "d3dx12.h"


class DirectX12Buffer
{
protected:
	ID3D12Resource* buffer;
	void* elementBuffer;
	void Map();
public:

	//@param pData 書き込むデータのポインタ
	//@param dataSize data全体のバイト単位のサイズ
	//Mapされた領域に引数のdataを書き込む
	void WriteBuffer(const void* pData, unsigned int dataSize);

	//@param pData データのポインタ
	//@param datasize データ一個のサイズ
	//@param datacount データの総数
	void WriteBuffer256Alignment(const void* pData, unsigned int datasize, unsigned int datacount);

	DirectX12Buffer();
	~DirectX12Buffer();
};

