#include "PMDController.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"
#include "ConstantBufferObject.h"
#include "TextureObj.h"
#include "VMDPlayer.h"
#include "PMDModel.h"
#include "DirectionalLight.h"


PMDController::PMDController()
{
}


PMDController::~PMDController()
{
}

void PMDController::Draw()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pso_notTex).Get());
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(rsi_pmd).Get());
	dirLight->SetLight();
	d12->SetCameraBuffer();
	model->SetIndexBuffer();
	model->SetVertexBuffer();
	SetBoneBuffer();
	model->SetMaterialBuffer();//この中でDrawまでやる
}

void PMDController::SetMotion(VMDMotion* motion)
{
	vmdPlayer->SetVMD(motion);
}

void PMDController::PlayMotion(bool loopFlag)
{
	vmdPlayer->Play(loopFlag);
}

void PMDController::StopMotion()
{
	vmdPlayer->Stop();
}

void PMDController::SetBoneBuffer()
{
	DX12CTRL_INSTANCE
	boneMatrixBuffer->WriteBuffer(&boneMatrix[0], static_cast<unsigned int>(sizeof(boneMatrix[0]) * boneMatrix.size()));
	boneMatrixBuffer->SetDescHeap();
	d12->GetCmdList()->SetGraphicsRootDescriptorTable(rpt_bonematrix, boneMatrixBuffer->GetGPUDescriptorHandle());
}

void PMDController::SetPosition(DirectX::XMFLOAT3& p)
{
	pos = p;
}

void PMDController::SetRota(DirectX::XMFLOAT3& rota)
{
	rotation = rota;
}

void PMDController::SetLight(std::shared_ptr<DirectionalLight> dlight)
{
	dirLight = dlight;
}