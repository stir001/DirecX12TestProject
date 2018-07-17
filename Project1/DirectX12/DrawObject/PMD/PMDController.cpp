#include "PMDController.h"
#include "Dx12Ctrl.h"
#include "RootSignatureObject.h"
#include "PipelineStateObject.h"
#include "ConstantBufferObject.h"
#include "TextureObject.h"
#include "VMDPlayer.h"
#include "PMDModel.h"
#include "DirectionalLight.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"
#include "ShaderResourceObject.h"
#include "Dx12DescriptorHeapObject.h"
#include "TextureObject.h"
#include "Dx12Camera.h"

PMDController::PMDController(std::shared_ptr<PMDModel>& model, std::shared_ptr<DirectionalLight>& dlight, const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	: DrawObjectController(name + "BundleCommnadList", dev, cmdList), mModel(model), mDirLight(dlight)
{
	mBoneMatrixBuffer.reset(new ConstantBufferObject("PMDBoneMatrixBuffer", Dx12Ctrl::Instance()->GetDev(), static_cast<unsigned int>(sizeof(DirectX::XMMATRIX) * mModel->mBoneDatas.size()), 1));
	mBoneMatrix.resize(mModel->mBoneDatas.size());
	for (auto& bm : mBoneMatrix) bm = DirectX::XMMatrixIdentity();
	mVmdPlayer.reset(new VMDPlayer(mModel->mBoneDatas, mModel->mBoneNode, mBoneMatrix));

	auto texObjs = mModel->GetTextureObjects();
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers;
	int constantBufferNum = 4;
	buffers.reserve(texObjs.size() + constantBufferNum);
	for (auto& tex : texObjs)
	{
		buffers.push_back(tex->GetShaderResource());
	}
	buffers.push_back(Dx12Ctrl::Instance()->GetCamera()->GetCameraBuffer());
	buffers.push_back(mDirLight->GetLightBuffer());
	buffers.push_back(mBoneMatrixBuffer);
	buffers.push_back(mModel->GetMaterialBuffer());
	std::string descName = name + "DescriptorHeap";
	mDescHeap.reset(new Dx12DescriptorHeapObject(descName, dev, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
}

PMDController::~PMDController()
{
}

void PMDController::Draw()
{
	mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	mCmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	//d12->SetCameraBuffer(d12->GetCmdList());
	mModel->SetIndexBuffer(mCmdList);
	mModel->SetVertexBuffer(mCmdList);
	SetBoneBuffer();
	//mModel->SetMaterialBuffer();//‚±‚Ì’†‚ÅDraw‚Ü‚Å‚â‚é
	SetMaterialBuffer();
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
	mBoneMatrixBuffer->WriteBuffer(&mBoneMatrix[0], static_cast<unsigned int>(sizeof(mBoneMatrix[0]) * mBoneMatrix.size()));
	//mBoneMatrixBuffer->SetDescHeap(d12->GetCmdList());
	//d12->GetCmdList()->SetGraphicsRootDescriptorTable(PMDModel::eROOT_PARAMATER_INDEX_BONE_MATRIX, mBoneMatrixBuffer->GetGPUViewHandle());
}

void PMDController::SetMaterialBuffer()
{
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	int offset = 0;
	mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {};// mTextureDescHeap->GetGPUDescriptorHandleForHeapStart();
	for (unsigned int i = 0; i < mModel->mMaterials.size(); i++)
	{
		if (mModel->mMaterials[i].texid != -1)
		{
			mCmdList->SetPipelineState(mSubPipeline->GetPipelineState().Get());
			if (mModel->GetTextureObjects()[mModel->mMaterials[i].texid] != nullptr)
			{
				//cmdList->SetDescriptorHeaps(1, mTextureDescHeap.GetAddressOf());
				//mTextureObjectects[mMaterials[i].texid]->SetBuffer(d12->GetCmdList(), eROOT_PARAMATER_INDEX_TEXTURE);
			}
		}
		else
		{
			mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
		}
		//mMaterialBuffer->SetDescHeap(d12->GetCmdList());
		//cmdList->SetGraphicsRootDescriptorTable(eROOT_PARAMATER_INDEX_MATERIAL, handle);
		mCmdList->DrawIndexedInstanced(mModel->mMaterials[i].indexCount, 1, offset, 0, 0);
		offset += mModel->mMaterials[i].indexCount;
	}
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

void PMDController::SetSubPipeLineState(std::shared_ptr<PipelineStateObject>& pipelineState)
{
	mSubPipeline = pipelineState;
}

void PMDController::SetSubRootsignature(std::shared_ptr<RootSignatureObject>& rootsiganture)
{
	mSubRootsignature = rootsiganture;
}
