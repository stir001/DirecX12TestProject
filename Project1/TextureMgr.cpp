#include "TextureMgr.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"



TextureMgr::TextureMgr()
{

}


TextureMgr::~TextureMgr()
{
}

//TextureObj* TextureMgr::LoadTexture(std::string path)
//{
//	int idx = path.rfind(".");
//	std::string str;
//	while (path[idx] != '\0')
//	{
//		str.push_back(path[idx++]);
//	}
//	str.push_back(path[idx]);
//
//	ImageData imgData = loader[str.data()]->LoadImageData(path);
//
//	TextureObj* texObj = new TextureObj();
//
//	D3D12_RESOURCE_DESC textureResourceDesc = {};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Width = imgData.width;
//	textureResourceDesc.Height = imgData.height;
//	textureResourceDesc.DepthOrArraySize = 1;
//	textureResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	textureResourceDesc.SampleDesc.Count = 1;
//	textureResourceDesc.SampleDesc.Quality = 0;
//	textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
//	textureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
//
//	D3D12_HEAP_PROPERTIES textureHeapProperties = {};
//	textureHeapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	textureHeapProperties.CreationNodeMask = 1;
//	textureHeapProperties.VisibleNodeMask = 1;
//
//
//	HRESULT result;
//
//	result = Dx12Ctrl::Instance()->GetDev()->CreateCommittedResource(
//		&textureHeapProperties,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texObj->textureBuffer));
//
//
//	D3D12_BOX box = {};
//	box.left = 0;
//	box.right = imgData.width;
//	box.top = 0;
//	box.bottom = imgData.height;
//	box.front = 0;
//	box.back = 1;
//
//	ID3D12GraphicsCommandList* cmdList = Dx12Ctrl::Instance()->GetCmdList();
//
//	result = texObj->textureBuffer->WriteToSubresource(0, &box, &imgData.rgbData[0], box.right * 4, 4 * box.bottom);
//
//	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texObj->textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
//	cmdList->Close();
//
//	Dx12Ctrl::Instance()->GetCmdQueue()->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
//
//	Dx12Ctrl::Instance()->GetCmdQueue()->Signal(Dx12Ctrl::Instance()->GetFence(), ++(Dx12Ctrl::Instance()->GetFenceValue()));
//	while (Dx12Ctrl::Instance()->GetFence()->GetCompletedValue() != Dx12Ctrl::Instance()->GetFenceValue()) {}
//
//	texture.push_back(texObj);
//
//	return texture.back();
//}

ID3D12DescriptorHeap* TextureMgr::CreateTextureDescHeap(std::string modelpath, int texturecount)
{
	DX12CTRL_INSTANCE
	auto itr = modelTextureDescHeaps.find(modelpath);
	if (itr != modelTextureDescHeaps.end()) return itr->second;

	D3D12_DESCRIPTOR_HEAP_DESC texDescHeapDesc = {};
	texDescHeapDesc.NumDescriptors = texturecount;
	texDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	texDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ID3D12DescriptorHeap* tex;
	d12->result = d12->GetDev()->CreateDescriptorHeap(&texDescHeapDesc, IID_PPV_ARGS(&tex));
	D12RESULTCHECK

	modelTextureDescHeaps[modelpath] = tex;
	
	return tex;
}

void TextureMgr::ReleaseDescHeap(std::string modelpath)
{
	auto itr = modelTextureDescHeaps.find(modelpath);
	if (itr == modelTextureDescHeaps.end()) return;
	itr->second->Release();
	modelTextureDescHeaps.erase(itr);
}

ID3D12DescriptorHeap* TextureMgr::GetDescHeap(std::string modelpath)
{
	auto itr = modelTextureDescHeaps.find(modelpath);
	if (itr == modelTextureDescHeaps.end()) return nullptr;
	return itr->second;
}
