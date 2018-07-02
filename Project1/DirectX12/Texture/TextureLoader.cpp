#include "TextureLoader.h"
#include "Dx12Ctrl.h"
#include "WICTextureLoader12.h"
#include "d3dx12.h"
#include "TextureObject.h"
#include "ShaderResourceObject.h"
#include "CharToWChar.h"

#include <tchar.h>
#include <DirectXMath.h>

TextureLoader* TextureLoader::mInstance = nullptr;

TextureLoader::TextureLoader()
{
	CoInitialize(nullptr);
}

TextureLoader::~TextureLoader()
{
	mTextures.clear();
}

void TextureLoader::Release(const std::string & filePath)
{
	mTextures.erase(filePath);
}

std::shared_ptr<TextureObject> TextureLoader::LoadTexture(const std::string& filepath)
{
	wchar_t* buf = nullptr;
	std::wstring wstrPath;
	ToWChar(&buf, filepath);
	wstrPath = buf;
	delete buf;


	auto tex = mTextures.find(filepath);
	if (tex != mTextures.end())
	{
		return tex->second;
	}
	std::shared_ptr<TextureObject> rtn(new TextureObject());
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
	DX12CTRL_INSTANCE
	d12->result = DirectX::LoadWICTextureFromFile(d12->GetDev().Get(), wstrPath.data(), &texture, rtn->mDecodedData, rtn->mSubresource);
	
	if (FAILED(d12->result))
	{
		#ifdef _DEBUG
		std::wstring outputstr = L"テクスチャパスが違うか対応していない拡張子なんやで\n FilePath:";
		outputstr += wstrPath;
		MessageBox(nullptr, outputstr.data() , L"CAUTION", MB_OK);
		#endif
		CreateNullTexture(rtn);
		rtn->mFilepath = filepath;
		rtn->mTextureName = GetTextureName(wstrPath);
		mTextures[filepath] = rtn;
		return rtn;
	}

	{
		rtn->mTextureName = GetTextureName(wstrPath);
		rtn->mShaderResource.reset(new ShaderResourceObject(rtn->mTextureName, texture));
	}

	D3D12_RESOURCE_DESC desc = texture->GetDesc();

	//CreateTexWriteToSubRrsource(rtn);
	CreateTexUpdateSubResources(rtn);

	rtn->mFilepath = filepath;
	mTextures[filepath] = rtn;
	return rtn;
}

void TextureLoader::CreateTexWriteToSubRrsource(std::shared_ptr<TextureObject>& inTex)
{
	Microsoft::WRL::ComPtr<ID3D12Resource>& dstResource = inTex->mShaderResource->GetBuffer();
	DX12CTRL_INSTANCE
	D3D12_RESOURCE_DESC desc = dstResource->GetDesc();

	//https://msdn.microsoft.com/ja-jp/library/windows/desktop/dn986725(v=vs.85).aspx
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	int count = dstResource.Reset();
	dstResource = nullptr;

	d12->result = d12->GetDev()->CreateCommittedResource(&heapProp
		, D3D12_HEAP_FLAG_NONE
		, &desc
		, D3D12_RESOURCE_STATE_GENERIC_READ
		, nullptr
		, IID_PPV_ARGS(&dstResource));


	D3D12_BOX box = {};
	box.left = 0;
	box.right = static_cast<UINT>(desc.Width);
	box.top = 0;
	box.bottom = desc.Height;
	box.front = 0;
	box.back = 1;

	d12->result = dstResource->WriteToSubresource(0, &box, inTex->mSubresource.pData, box.right * 4, box.bottom * 4);

	d12->GetCmdList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dstResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

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
	d12->GetCmdList()->Reset(d12->GetCmdAllocator().Get(), nullptr);
}

void TextureLoader::CreateTexUpdateSubResources(std::shared_ptr<TextureObject>& inTex)
{
	DX12CTRL_INSTANCE

	Microsoft::WRL::ComPtr<ID3D12Resource> updateBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>& dstResource = inTex->mShaderResource->GetBuffer();

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC uploadDesc;
	uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	uploadDesc.Alignment = 0;
	uploadDesc.Width = GetRequiredIntermediateSize(dstResource.Get(), 0, 1);
	uploadDesc.Height = 1;
	uploadDesc.DepthOrArraySize = 1;
	uploadDesc.MipLevels = 1;
	uploadDesc.Format = DXGI_FORMAT_UNKNOWN;
	uploadDesc.SampleDesc.Count = 1;
	uploadDesc.SampleDesc.Quality = 0;
	uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	uploadDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	d12->result = d12->GetDev()->CreateCommittedResource(
		&heapProp, 
		D3D12_HEAP_FLAG_NONE,
		&uploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&updateBuffer));

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = d12->GetCmdList();

	UINT64 num = UpdateSubresources(cmdList.Get(),
		dstResource.Get(),
		updateBuffer.Get(),
		0,
		0,
		1,
		&inTex->mSubresource);


	//https://msdn.microsoft.com/en-us/library/windows/desktop/dn986740(v=vs.85).aspx
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dstResource.Get();
	barrier.Transition.Subresource = 0;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	cmdList->ResourceBarrier(1, &barrier);

	cmdList->Close();

	{
		ID3D12GraphicsCommandList* t_cmdList = cmdList.Get();
		d12->GetCmdQueue()->ExecuteCommandLists(1, (ID3D12CommandList* const*)(&t_cmdList));
	}

	d12->CmdQueueSignal();

	d12->GetCmdAllocator()->Reset();
	cmdList->Reset(d12->GetCmdAllocator().Get(),nullptr);
}

void TextureLoader::CreateNullTexture(std::shared_ptr<TextureObject>& inTex)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> dstResource;
	DX12CTRL_INSTANCE

	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = 16;
	desc.Height = 16;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	d12->result = d12->GetDev()->CreateCommittedResource(&heapProp
		, D3D12_HEAP_FLAG_NONE
		, &desc
		, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		, nullptr
		, IID_PPV_ARGS(&dstResource));

	inTex->mShaderResource.reset(new ShaderResourceObject(inTex->mTextureName, dstResource));
}

std::string TextureLoader::GetTextureName(const std::wstring& filePath)
{
	size_t size = filePath.rfind('/');
	
	std::wstring wstrbuff(filePath.begin() + size + 1, filePath.end());

	char* charbuf = nullptr;

	ToChar(&charbuf, sizeof(char) * wstrbuff.size() + 1, wstrbuff.data(), wstrbuff.size() + 1);

	std::string rtn = charbuf;

	delete charbuf;

	return rtn;
}