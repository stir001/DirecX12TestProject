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
#include "Dx12CommandList.h"

PMDController::PMDController(std::shared_ptr<PMDModel>& model, std::shared_ptr<DirectionalLight>& dlight, const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	: DrawObjectController(name + "BundleCommnadList", dev, cmdList), mModel(model), mDirLight(dlight), mBundleUpdate(&PMDController::UpdateBundle)
{
	mBoneMatrixBuffer.reset(new ConstantBufferObject("PMDBoneMatrixBuffer", Dx12Ctrl::Instance()->GetDev(), static_cast<unsigned int>(sizeof(DirectX::XMMATRIX) * mModel->mBoneDatas.size()), 1));
	mBoneMatrix.resize(mModel->mBoneDatas.size());
	for (auto& bm : mBoneMatrix) bm = DirectX::XMMatrixIdentity();
	mBoneMatrixBuffer->WriteBuffer(&mBoneMatrix[0], static_cast<unsigned int>(sizeof(DirectX::XMMATRIX) * mModel->mBoneDatas.size()));
	mVmdPlayer.reset(new VMDPlayer(mModel->mBoneDatas, mModel->mBoneNode, mBoneMatrix));

	CreateDescriptorHeap(dev, name);
}

PMDController::~PMDController()
{
}

void PMDController::Draw()
{
	(this->*mBundleUpdate)();
	mDescHeap->SetDescriptorHeap(mCmdList);
	mCmdList->ExecuteBundle(mBundleCmdList->GetCommandList().Get());
	//mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	//mCmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());

	//mModel->SetIndexBuffer(mCmdList);
	//mModel->SetVertexBuffer(mCmdList);
	//mDescHeap->SetDescriptorHeap(mCmdList);

	//DrawWhileSetTable(mCmdList);
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


void PMDController::DrawWhileSetTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	unsigned int indexOffset = 0;
	unsigned int offsetCount = 0;

	for (auto& material : mModel->mMaterials)
	{
		if (material.texid != -1)
		{
			SetTexture(cmdList, material);
		}
		else
		{
			cmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
			cmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
		}
		SetConstantBuffers(cmdList);
		SetMaterial(cmdList, mModel->GetTextureObjects().size() + 3, offsetCount);
		cmdList->DrawIndexedInstanced(material.indexCount, 1, indexOffset, 0, 0);
		indexOffset += material.indexCount;
		++offsetCount;
	}
}

void PMDController::SetTexture(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, PMDMaterial& material)
{
	cmdList->SetPipelineState(mSubPipeline->GetPipelineState().Get());
	cmdList->SetGraphicsRootSignature(mSubRootsignature->GetRootSignature().Get());
	mDescHeap->SetGprahicsDescriptorTable(cmdList, material.texid, PMDModel::eROOT_PARAMATER_INDEX_TEXTURE);
}

void PMDController::SetMaterial(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount)
{
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex, PMDModel::eROOT_PARAMATER_INDEX_MATERIAL, offsetCount);
}

void PMDController::CreateDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string & name)
{
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

void PMDController::SetConstantBuffers(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	unsigned int resourceIndex = mModel->GetTextureObjects().size();
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_LIGHT);
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_BONE_MATRIX);
}

void PMDController::UpdateBundle()
{
	mBundleCmdList->Reset();
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& bundle = mBundleCmdList->GetCommandList();
	bundle->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	bundle->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());

	mModel->SetIndexBuffer(bundle);
	mModel->SetVertexBuffer(bundle);
	mDescHeap->SetDescriptorHeap(bundle);

	DrawWhileSetTable(bundle);
	mBundleCmdList->Close();
}

void PMDController::NonUpdateBundle()
{

}