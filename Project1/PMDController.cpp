#include "PMDController.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"
#include "ConstantBufferObject.h"
#include "TextureObj.h"
#include "VMDPlayer.h"
#include "PMDModel.h"


PMDController::PMDController()
{
}


PMDController::~PMDController()
{
}

void PMDController::Draw()
{
	model->SetIndexBuffer();
	model->SetVertexBuffer();
	SetBoneBuffer();
	model->SetMaterialBuffer();//‚±‚Ì’†‚ÅDraw‚Ü‚Å‚â‚é
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