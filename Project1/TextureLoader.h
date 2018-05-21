#pragma once
#include <memory>
#include <map>
#include <string>
#include <d3d12.h>

struct ID3D12Resource;
struct D3D12_SUBRESOURCE_DATA;
struct ID3D12DescriptorHeap;
class TextureObj;
class TextureMgr;

class TextureLoader
{
private:
	std::map<std::wstring, TextureObj*> textures;
	TextureMgr* texMgr;

	void CreateTexWriteToSubRrsource(TextureObj*& inTex);
	void CreateTexUpdateSubResources(TextureObj*& inTex);

public:
	ID3D12DescriptorHeap* CreateTexDescHeap(std::string modelpath, int texcount);
	TextureObj* LoadTexture(std::wstring filepath,D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle);
	TextureLoader();
	~TextureLoader();
};

