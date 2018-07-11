#include "PMDController.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"
#include "PipelineStateObject.h"
#include "ConstantBufferObject.h"
#include "TextureObject.h"
#include "VMDPlayer.h"
#include "PMDModel.h"
#include "DirectionalLight.h"


PMDController::PMDController(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList): DrawObjectController(name + "BundleCommnadList", dev, cmdList)
{
}

PMDController::~PMDController()
{
}

void PMDController::Draw()
{
	mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	mCmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	//d12->SetCameraBuffer(d12->GetCmdList());
	mModel->SetIndexBuffer(d12->GetCmdList());
	mModel->SetVertexBuffer(d12->GetCmdList());
	SetBoneBuffer();
	mModel->SetMaterialBuffer();//���̒���Draw�܂ł��
}

void PMDController::SetMotion(std::shared_ptr<VMDMotion> motion)
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