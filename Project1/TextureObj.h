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
	float gamma;
public:
	int GetWidth() const;
	int GetHeight() const;
	void SetBuffer() const;
	float GetGamma() const;//γ補正の値を返す,現在は適当な値を返している
	TextureObj();
	~TextureObj();
};

