#include "DrawObjectLoader.h"
#include "ShaderCompiler.h"
#include "File.h"
#include "PipelineStateObject.h"
#include <wpframework.h>

DrawObjectLoader::DrawObjectLoader() :mPipelinestate(nullptr), mRootsignature(nullptr)
{
	setlocale(LC_ALL, "japanese");
}

DrawObjectLoader::~DrawObjectLoader()
{
	delete mFp;
}

void DrawObjectLoader::GetRelativePath(const std::string& path)
{
	mRelativePath.resize(path.size());
	size_t num;
	mbstowcs_s(&num, &mRelativePath[0], mRelativePath.size(), &path[0], mRelativePath.size() - 1);
	while (mRelativePath.size())
	{
		if (mRelativePath.back() == '/' || mRelativePath.back() == '\\') break;
		mRelativePath.pop_back();
	}
}

void DrawObjectLoader::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAllocator.Get(), mPipelinestate->GetPipelineState().Get(), IID_PPV_ARGS(&mCmdList));
}

void DrawObjectLoader::CreateCommandAllocator(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCmdAllocator));
}

void DrawObjectLoader::SetRenderingCommnadList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	mCmdList = cmdList;
}