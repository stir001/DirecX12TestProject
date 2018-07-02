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

void DrawObjectLoader::SetRenderingCommnadList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	mCmdList = cmdList;
}