#pragma once
#include <memory>
#include <string>
#include <d3d12.h>


class TextureObj
{
	friend class TextureLoader;
private:
	std::wstring filepath;
	ID3D12Resource* textureBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	std::unique_ptr<uint8_t[]> decodedData;
	D3D12_SUBRESOURCE_DATA subresource;
public:
	int GetWidth() const;
	int GetHeight() const;
	void SetBuffer() const;
	void SetBuffer(int rootparamIndex) const;
	TextureObj();
	~TextureObj();
};

