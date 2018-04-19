#pragma once

#include <vector>
#include <map>
#include <d3d12.h>
#include <string>

class TextureMgr
{
private:
	std::map<std::string, ID3D12DescriptorHeap*> modelTextureDescHeaps;
public:
	ID3D12DescriptorHeap* GetDescHeap(std::string modelpath);
	ID3D12DescriptorHeap* CreateTextureDescHeap(std::string modelpath,int texturecont);
	void ReleaseDescHeap(std::string modelpath);
	TextureMgr();
	~TextureMgr();
};

