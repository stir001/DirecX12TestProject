#pragma once
#include "ConstantBufferObject.h"
#include <vector>
#include <functional>
#include <d3d12.h>
#include <wrl.h>

template <typename T>
class AdditionalConstatntBuffer : public ConstantBufferObject
{
public:
	AdditionalConstatntBuffer(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Device>& dev, T* data)
		: ConstantBufferObject(name, dev, sizeof(*data), 1),
		mData(data)
	{
		mWriteBuffer = [&]() {
			WriteBuffer256Alignment(mData, sizeof(*mData), 1);
		};
		mWriteBuffer();
		SetUpdate(mWriteBuffer);
	}

	AdditionalConstatntBuffer(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::vector<T>* data)
		:ConstantBufferObject(name, dev, sizeof(data[0], data.size())),
		mVectorDatas(data)
	{
		mWriteBuffer = [&]() {
			WriteBuffer256Alignment(&(*mVectorDatas)[0], sizeof((*mVectorDatas)[0]), (*mVectorDatas).size());
		};

		mWriteBuffer();
		SetUpdate(mWriteBuffer);
	}

	~AdditionalConstatntBuffer()
	{
	}

private:
	std::function<void(void)> mWriteBuffer;

	T* mData;
	std::vector<T>* mVectorDatas;
};

