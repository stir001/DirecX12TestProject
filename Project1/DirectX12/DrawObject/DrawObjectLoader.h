#pragma once
#include "ShaderDatasStructure.h"

#include <string>
#include <memory>
#include <d3d12.h>

class File;
class ShaderCompiler;
class PipelineStateObject;
class RootSignatureObject;
struct ShaderDatas;

class DrawObjectLoader
{
public:
	DrawObjectLoader();
	virtual ~DrawObjectLoader();


	void SetRenderingCommnadList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
protected:
	File* mFp;
	std::string mRelativePath;//‘Š‘ÎƒpƒX•Û‘¶—p

	ShaderDatas mShader;
	std::shared_ptr<PipelineStateObject> mPipelinestate;
	std::shared_ptr<RootSignatureObject> mRootsignature;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	//Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdAllocator;

	void GetRelativePath(const std::string& path);

	virtual void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev) = 0;
	virtual void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev) = 0;
private:
};

