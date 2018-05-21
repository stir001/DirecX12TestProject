#include "FbxModelController.h"
#include "FbxModel.h"
#include "Dx12Ctrl.h"
#include "DirectionalLight.h"

using namespace Fbx;

FbxModelController::FbxModelController()
{
}

FbxModelController::FbxModelController(std::shared_ptr<FbxModel> model)
{
	mModel = model;
}

FbxModelController::~FbxModelController()
{
}

void FbxModelController::Draw()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pos_fbx).Get());
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(rsi_fbx).Get());
	d12->SetCameraBuffer();
	mDirLight->SetLight();
	mModel->SetIndexBuffer();
	mModel->SetVertexBuffer();
	mModel->SetTexture();
	d12->GetCmdList()->DrawIndexedInstanced(15, 1, 0, 0, 0);
}

void FbxModelController::SetLight(std::shared_ptr<DirectionalLight> dirlight)
{
	mDirLight = dirlight;
}

