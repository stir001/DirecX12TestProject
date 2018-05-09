#include "TextureLoader.h"
#include "Dx12Ctrl.h"
#include "WICTextureLoader12.h"
#include "d3dx12.h"
#include "TextureObj.h"
#include "TextureMgr.h"

#include <tchar.h>
#include <DirectXMath.h>

TextureLoader::TextureLoader()
{
	CoInitialize(nullptr);
	texMgr = new TextureMgr();
}


TextureLoader::~TextureLoader()
{
	delete texMgr;
}

TextureObj* TextureLoader::LoadTexture(std::wstring filepath, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle)
{
	auto tex = textures.find(filepath);
	if (tex != textures.end()) return tex->second;
	TextureObj* rtn = new TextureObj();
	rtn->cpuHandle = cpuHandle;
	rtn->gpuHandle = gpuhandle;
	DX12CTRL_INSTANCE
	d12->result = DirectX::LoadWICTextureFromFile(d12->GetDev().Get(),  filepath.data(), &rtn->textureBuffer, rtn->decodedData, rtn->subresource);

	if (FAILED(d12->result))
	{
		delete rtn;
		return rtn = nullptr;
	}

	D3D12_RESOURCE_DESC desc = rtn->textureBuffer->GetDesc();

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	int count = rtn->textureBuffer->Release();
	rtn->textureBuffer = nullptr;

	d12->result = d12->GetDev()->CreateCommittedResource(&heapProp
		, D3D12_HEAP_FLAG_NONE
		, &desc
		, D3D12_RESOURCE_STATE_GENERIC_READ
		, nullptr
		, IID_PPV_ARGS(&rtn->textureBuffer));


	D3D12_BOX box = {};
	box.left = 0;
	box.right = static_cast<UINT>(desc.Width);
	box.top = 0;
	box.bottom = desc.Height;
	box.front = 0;
	box.back = 1;

	rtn->textureBuffer->WriteToSubresource(0, &box, rtn->subresource.pData, box.right * 4, box.bottom * 4);

	D3D12_SHADER_RESOURCE_VIEW_DESC wicSrvDesc = {};
	wicSrvDesc.Format = desc.Format;
	wicSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	wicSrvDesc.Texture2D.MipLevels = 1;
	wicSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	d12->GetCmdList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rtn->textureBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	d12->GetDev()->CreateShaderResourceView(rtn->textureBuffer, &wicSrvDesc, rtn->cpuHandle);

	{
		//close()を忘れてエラーをはいていた
		//バリアの張り方がわるいのかバリアでエラーをはくので一部バリアを書いていない(書く必要もないように感じるが...
		//改善対象
		//cmdList変数の寿命管理のためブロックを作っている
		ID3D12GraphicsCommandList* cmdList = d12->GetCmdList().Get();
		cmdList->Close();
		d12->GetCmdQueue()->ExecuteCommandLists(1, (ID3D12CommandList* const*)(&cmdList));

	}

	d12->CmdQueueSignal();
	d12->GetCmdList()->Reset(d12->GetCmdAllocator().Get(), d12->GetPiplineState(pso_notTex).Get());
	rtn->filepath = filepath;
	textures[filepath] = rtn;
	return rtn;
}

ID3D12DescriptorHeap* TextureLoader::CreateTexDescHeap(std::string modelpath, int texcount)
{
	return texMgr->CreateTextureDescHeap(modelpath, texcount);
}