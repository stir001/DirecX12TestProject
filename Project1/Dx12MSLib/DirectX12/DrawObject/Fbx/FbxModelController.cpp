#include "stdafx.h"
#include "FbxModelController.h"
#include "FbxModel.h"
#include "FbxMotionPlayer.h"
#include "Master/Dx12Ctrl.h"
#include "Light/LightObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "PipelineState/PipelineStateObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/DepthBufferObject.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Camera/Dx12Camera.h"
#include "Texture/TextureObject.h"
#include "Buffer/ShaderResourceObject.h"
#include "CommandList/Dx12CommandList.h"

using namespace Fbx;
using namespace DirectX;

FbxModelController::FbxModelController(std::shared_ptr<FbxModel>& model,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
	std::shared_ptr<PipelineStateObject>& pipelinestate,
	std::shared_ptr<RootSignatureObject>& rootsignature)
	:DrawController3D(model->GetModelName() ,dev, cmdList) 
	,mModel(model), mColor(0.8f, 0.8f, 0.8f)
	, mBundleUpdate(&FbxModelController::UpdateBundle)
{
	mPipelinestate = pipelinestate;
	mRootsignature = rootsignature;
	DirectX::XMStoreFloat4x4(&mModelMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMMatrixIdentity());
	XMStoreFloat4(&mQuaternion, XMQuaternionIdentity());
	std::string cbufferName = mModel->GetModelName();

	mModelMatrixBuffer = std::make_shared<ConstantBufferObject>(cbufferName + "MatrixBuffer", dev, static_cast<unsigned int>(sizeof(XMMATRIX)), 1);

	mVertexElements.resize(mModel->mVertexes.size());
	for (unsigned int i = 0; i < static_cast<unsigned int>(mVertexElements.size()); ++i)
	{
		mVertexElements[i].pos = mModel->mVertexes[i].pos;
		mVertexElements[i].normal = mModel->mVertexes[i].normal;
		mVertexElements[i].texCoord = mModel->mVertexes[i].texCoord;
		DirectX::XMStoreFloat4x4(&mVertexElements[i].vertexMatrix, DirectX::XMMatrixIdentity());
	}

	mCtrlVertexBuffer = (std::make_shared<VertexBufferObject>(cbufferName + "VertexBuffer", dev, static_cast<unsigned int>(sizeof(mVertexElements[0])), static_cast<unsigned int>(mVertexElements.size())));
	UpdateVertex();

	mMotionPlayer = std::make_shared<FbxMotionPlayer>(mModel->mBones, mModel->mVertexes, mVertexElements);

	mCameraBuffer = Dx12Ctrl::Instance().GetCamera()->GetCameraBuffer();

	UpdateMatrix();
}

FbxModelController::~FbxModelController()
{
}

void FbxModelController::Draw()
{
	UpdateVertex();
	(this->*mBundleUpdate)();
	mCmdList->SetDescriptorHeaps(1, mDescHeap->GetDescriptorHeap().GetAddressOf());
	mCmdList->ExecuteBundle(mBundleCmdList->GetCommandList().Get());
	
	for (auto& buffer : mAddConstantBuffers)
	{
		buffer->UpdateBuffer();
	}
}

void FbxModelController::SetLight(std::shared_ptr<LightObject> dirlight)
{
	mDirLightBuffer = dirlight->GetLightBuffer();
	UpdateDescriptorHeap();
	mBundleUpdate = &FbxModelController::UpdateBundle;
}

void FbxModelController::SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = (rootsignature);
	mBundleUpdate = &FbxModelController::UpdateBundle;
}

void FbxModelController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mPipelinestate = (pipelinestate);
	mBundleUpdate = &FbxModelController::UpdateBundle;
}

void FbxModelController::SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	mCmdList = cmdList;
}

void FbxModelController::SetMotion(std::shared_ptr<FbxMotionData>& motion,bool isLoop)
{
	mMotionPlayer->SetMotion(motion, isLoop);
}

void FbxModelController::StopMotion() const
{
	mMotionPlayer->StopMotion();
}

void FbxModelController::RestartMotion() const
{
	mMotionPlayer->ReStartMotion();
}

bool FbxModelController::IsMotionEnd() const
{
	return mMotionPlayer->IsEnd();
}

void  FbxModelController::AddConstantBuffer(std::shared_ptr<ConstantBufferObject>& buffer)
{
	mAddConstantBuffers.push_back(buffer);
	UpdateDescriptorHeap();
	mBundleUpdate = &FbxModelController::UpdateBundle;
}

std::string FbxModelController::GetModelName() const
{
	return mModel->GetModelName();
}

std::string FbxModelController::GetModelPath() const
{
	return mModel->GetModelPath();
}

void FbxModelController::UpdateVertex()
{
	mCtrlVertexBuffer->WriteBuffer(&mVertexElements[0], static_cast<unsigned int>(sizeof(mVertexElements[0]) * mVertexElements.size()));
}

void FbxModelController::UpdateBundle()
{
	mBundleCmdList->Reset();
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& bundle = mBundleCmdList->GetCommandList();
	bundle->SetDescriptorHeaps(1, mDescHeap->GetDescriptorHeap().GetAddressOf());
	bundle->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	bundle->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	mModel->SetIndexBuffer(bundle);
	mCtrlVertexBuffer->SetBuffer(bundle);

	bundle->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	unsigned int resourceIndex = 0;

	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_LIGHT);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_MATRIX);
	unsigned int offsetIndex = 0;
	for (auto& buffer : mAddConstantBuffers)
	{
		mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_MAX + offsetIndex++);
	}

	unsigned int indexOffset = 0;
	for (auto& mat : mModel->mMaterials)
	{
		for (unsigned int i = 0; i < Fbx::FbxModel::eROOT_PARAMATER_INDEX_TRANSPARENCY_FACTOR + 1; ++i)
		{
			mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, static_cast<Fbx::FbxModel::eROOT_PARAMATER_INDEX>(i));
		}
		bundle->DrawIndexedInstanced(static_cast<UINT>(mat.drawIndexNum), 1, indexOffset, 0, 0);
		indexOffset += mat.drawIndexNum;
	}
	
	bundle->Close();

	mBundleUpdate = &FbxModelController::NonBundleUpdate;
}

void FbxModelController::NonBundleUpdate()
{
}

void FbxModelController::UpdateDescriptorHeap()
{
	std::vector<std::shared_ptr<Dx12BufferObject>> bufferObj;
	bufferObj.reserve((Fbx::FbxModel::eROOT_PARAMATER_INDEX_TRANSPARENCY_FACTOR + 1) * mModel->mMaterials.size() + (Fbx::FbxModel::eROOT_PARAMATER_INDEX_TRANSPARENCY_FACTOR - (Fbx::FbxModel::eROOT_PARAMATER_INDEX_TRANSPARENCY_FACTOR + 1)) + mAddConstantBuffers.size());
	bufferObj.push_back(mCameraBuffer);
	bufferObj.push_back(mDirLightBuffer);
	bufferObj.push_back(mModelMatrixBuffer);
	for (auto& addcbuf : mAddConstantBuffers)
	{
		bufferObj.push_back(addcbuf);
	}
	for (auto& mat : mModel->mMaterials)
	{
		for (auto& tex : mat.textures)
		{
			bufferObj.push_back(tex->GetShaderResource());
		}
	}

	std::string name = mModel->GetModelName();
	name += "DescriptorHeap";
	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>(name, Dx12Ctrl::Instance().GetDev(), bufferObj, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}