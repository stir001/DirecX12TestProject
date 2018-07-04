#include "DrawObjectController.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"
#include "Dx12CommandList.h"


DrawObjectController::DrawObjectController(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	:mCmdList(cmdList)
{
	mBundleCmdList.reset(new Dx12CommandList(name, dev,D3D12_COMMAND_LIST_TYPE_BUNDLE));
}

DrawObjectController::~DrawObjectController()
{
	
}

void DrawObjectController::SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
}

void DrawObjectController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mPipelinestate = pipelinestate;
}

void DrawObjectController::SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdlist)
{
	mCmdList = cmdlist;
}
