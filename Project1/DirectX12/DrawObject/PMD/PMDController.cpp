#include "PMDController.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"
#include "ConstantBufferObject.h"
#include "TextureObject.h"
#include "VMDPlayer.h"
#include "PMDModel.h"
#include "DirectionalLight.h"


PMDController::PMDController()//:DrawObjectController(Dx12Ctrl::Instance()->GetCmdList())
{
}

PMDController::~PMDController()
{
}

void PMDController::Draw()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPipelineState(pso_notTex).Get());
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(rsi_pmd).Get());
	mDirLight->SetLight(d12->GetCmdList());
	//d12->SetCameraBuffer(d12->GetCmdList());
	mModel->SetIndexBuffer(d12->GetCmdList());
	mModel->SetVertexBuffer(d12->GetCmdList());
	SetBoneBuffer();
	mModel->SetMaterialBuffer();//‚±‚Ì’†‚ÅDraw‚Ü‚Å‚â‚é
}

void PMDController::SetMotion(VMDMotion* motion)
{
	mVmdPlayer->SetVMD(motion);
}

void PMDController::PlayMotion(bool loopFlag)
{
	mVmdPlayer->Play(loopFlag);
}

void PMDController::StopMotion()
{
	mVmdPlayer->Stop();
}

void PMDController::SetBoneBuffer()
{
	DX12CTRL_INSTANCE
	mBoneMatrixBuffer->WriteBuffer(&mBoneMatrix[0], static_cast<unsigned int>(sizeof(mBoneMatrix[0]) * mBoneMatrix.size()));
	//mBoneMatrixBuffer->SetDescHeap(d12->GetCmdList());
	//d12->GetCmdList()->SetGraphicsRootDescriptorTable(PMDModel::eROOT_PARAMATER_INDEX_BONE_MATRIX, mBoneMatrixBuffer->GetGPUViewHandle());
}

void PMDController::SetPosition(DirectX::XMFLOAT3& p)
{
	mPos = p;
}

void PMDController::SetRota(DirectX::XMFLOAT3& rota)
{
	mRotation = rota;
}

void PMDController::SetLight(std::shared_ptr<DirectionalLight> dlight)
{
	mDirLight = dlight;
}