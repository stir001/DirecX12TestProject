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
	d12.result = DirectX::LoadWICTextureFromFile(d12.GetDev().Get(),  filepath.data(), &rtn->textureBuffer, rtn->decodedData, rtn->subresource);

	if (FAILED(d12.result))
	{
		delete rtn;
		return rtn = nullptr;
	}

	D3D12_RESOURCE_DESC desc = rtn->textureBuffer->GetDesc();

	//CreateTexWriteToSubRrsource(rtn);
	CreateTexUpdateSubResources(rtn);

	rtn->filepath = filepath;
	textures[filepath] = rtn;
	return rtn;
}

ID3D12DescriptorHeap* TextureLoader::CreateTexDescHeap(std::string modelpath, int texcount)
{
	return texMgr->CreateTextureDescHeap(modelpath, texcount);
}

void TextureLoader::CreateTexWriteToSubRrsource(TextureObj*& inTex)
{
	DX12CTRL_INSTANCE
	D3D12_RESOURCE_DESC desc = inTex->textureBuffer->GetDesc();

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	int count = inTex->textureBuffer->Release();
	inTex->textureBuffer = nullptr;

	d12.result = d12.GetDev()->CreateCommittedResource(&heapProp
		, D3D12_HEAP_FLAG_NONE
		, &desc
		, D3D12_RESOURCE_STATE_GENERIC_READ
		, nullptr
		, IID_PPV_ARGS(&inTex->textureBuffer));


	D3D12_BOX box = {};
	box.left = 0;
	box.right = static_cast<UINT>(desc.Width);
	box.top = 0;
	box.bottom = desc.Height;
	box.front = 0;
	box.back = 1;

	d12.result = inTex->textureBuffer->WriteToSubresource(0, &box, inTex->subresource.pData, box.right * 4, box.bottom * 4);

	D3D12_SHADER_RESOURCE_VIEW_DESC wicSrvDesc = {};
	wicSrvDesc.Format = desc.Format;
	wicSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	wicSrvDesc.Texture2D.MipLevels = 1;
	wicSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	d12.GetCmdList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(inTex->textureBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	d12.GetDev()->CreateShaderResourceView(inTex->textureBuffer, &wicSrvDesc, inTex->cpuHandle);

	{
		//close()を忘れてエラーをはいていた
		//バリアの張り方がわるいのかバリアでエラーをはくので一部バリアを書いていない(書く必要もないように感じるが...
		//改善対象
		//cmdList変数の寿命管理のためブロックを作っている
		ID3D12GraphicsCommandList* cmdList = d12.GetCmdList().Get();
		cmdList->Close();
		d12.GetCmdQueue()->ExecuteCommandLists(1, (ID3D12CommandList* const*)(&cmdList));

	}

	d12.CmdQueueSignal();
	d12.GetCmdList()->Reset(d12.GetCmdAllocator().Get(), d12.GetPiplineState(pso_notTex).Get());
}

void TextureLoader::CreateTexUpdateSubResources(TextureObj*& inTex)
{
	DX12CTRL_INSTANCE

	ID3D12Resource* updateBuffer;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC uploadDesc;
	uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	uploadDesc.Alignment = 0;
	uploadDesc.Width = GetRequiredIntermediateSize(inTex->textureBuffer,0,1);
	uploadDesc.Height = 1;
	uploadDesc.DepthOrArraySize = 1;
	uploadDesc.MipLevels = 1;
	uploadDesc.Format = DXGI_FORMAT_UNKNOWN;
	uploadDesc.SampleDesc.Count = 1;
	uploadDesc.SampleDesc.Quality = 0;
	uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	uploadDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	d12.result = d12.GetDev()->CreateCommittedResource(
		&heapProp, 
		D3D12_HEAP_FLAG_NONE,
		&uploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&updateBuffer));

	UINT64 num = UpdateSubresources(d12.GetCmdList().Get(),
		inTex->textureBuffer,
		updateBuffer,
		0,
		0,
		1,
		&inTex->subresource);

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = inTex->textureBuffer;
	barrier.Transition.Subresource = 0;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d12.GetCmdList()->ResourceBarrier(1, &barrier);

	D3D12_SHADER_RESOURCE_VIEW_DESC wicSrvDesc = {};
	wicSrvDesc.Format = inTex->textureBuffer->GetDesc().Format;
	wicSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	wicSrvDesc.Texture2D.MipLevels = 1;
	wicSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	d12.GetDev()->CreateShaderResourceView(inTex->textureBuffer, &wicSrvDesc, inTex->cpuHandle);

	d12.GetCmdList()->Close();

	{
		ID3D12GraphicsCommandList* cmdList = d12.GetCmdList().Get();
		d12.GetCmdQueue()->ExecuteCommandLists(1, (ID3D12CommandList* const*)(&cmdList));
	}

	d12.CmdQueueSignal();

	d12.GetCmdList()->Reset(d12.GetCmdAllocator().Get(), d12.GetPiplineState(pso_exitTex ).Get());
}