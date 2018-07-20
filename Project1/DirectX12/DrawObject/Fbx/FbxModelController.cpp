#include "FbxModelController.h"
#include "FbxModel.h"
#include "Dx12Ctrl.h"
#include "DirectionalLight.h"
#include "ConstantBufferObject.h"
#include "PipelineStateObject.h"
#include "RootSignatureObject.h"
#include "FbxMotionPlayer.h"
#include "VertexBufferObject.h"
#include "DepthBufferObject.h"
#include "RenderingPathManager.h"
#include "Dx12DescriptorHeapObject.h"
#include "Dx12Camera.h"
#include "TextureObject.h"
#include "ShaderResourceObject.h"
#include "Dx12CommandList.h"

using namespace Fbx;
using namespace DirectX;

const unsigned int DEFALUT_RESOURCE_NUM = 6;

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
	cbufferName += "MatrixBuffer";
	mModelMatrixBuffer.reset(new ConstantBufferObject(cbufferName, dev,sizeof(XMMATRIX), 1));

	cbufferName = mModel->GetModelName();
	cbufferName += "ColorBuffer";

	mVertexElements.resize(mModel->mVertexes.size());
	for (unsigned int i = 0; i < (mVertexElements.size()); ++i)
	{
		mVertexElements[i].pos = mModel->mVertexes[i].pos;
		mVertexElements[i].normal = mModel->mVertexes[i].normal;
		mVertexElements[i].texCoord = mModel->mVertexes[i].texCoord;
		DirectX::XMStoreFloat4x4(&mVertexElements[i].vertexMatrix, DirectX::XMMatrixIdentity());
	}

	cbufferName = mModel->GetModelName();
	cbufferName += "VertexBuffer";

	mCtrlVertexBuffer.reset(new VertexBufferObject(cbufferName,dev,sizeof(mVertexElements[0]), mVertexElements.size()));
	UpdateVertex();

	mMotionPlayer = new FbxMotionPlayer(mModel->mBones, mModel->mVertexes, mVertexElements);

	mCameraBuffer = Dx12Ctrl::Instance()->GetCamera()->GetCameraBuffer();

	mBundleCmdList->Close();
	UpdateMatrix();
}

FbxModelController::~FbxModelController()
{
	delete mMotionPlayer;
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

void FbxModelController::SetLight(std::shared_ptr<DirectionalLight>& dirlight)
{
	mDirLightBuffer = dirlight->GetLightBuffer();
	UpdateDescriptorHeap();
	mBundleUpdate = &FbxModelController::UpdateBundle;
}

//void FbxModelController::SetPositon(const DirectX::XMFLOAT3& pos)
//{
//	mPos = pos;
//	UpdateMatrix();
//}
//
//void FbxModelController::SetScale(float scale)
//{
//	mScale = scale;
//	UpdateMatrix();
//}
//
//void  FbxModelController::AddRotaX(float deg)
//{
//	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * XMMatrixRotationX(XMConvertToRadians(deg)));
//	UpdateMatrix();
//}
//
//void  FbxModelController::AddRotaY(float deg)
//{
//	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * XMMatrixRotationY(XMConvertToRadians(deg)));
//	UpdateMatrix();
//}
//
//void  FbxModelController::AddRotaZ(float deg)
//{
//	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * XMMatrixRotationZ(XMConvertToRadians(deg)));
//	UpdateMatrix();
//}
//
//void FbxModelController::SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion)
//{
//	mQuaternion = quaternion;
//	UpdateMatrix();
//}

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
	mCtrlVertexBuffer->WriteBuffer(&mVertexElements[0], sizeof(mVertexElements[0]) * mVertexElements.size());
}

//void FbxModelController::UpdateMatrix()
//{
//	XMMATRIX mat = XMMatrixIdentity();
//	XMVECTOR q = XMLoadFloat4(&mQuaternion);
//	mat *= XMMatrixRotationQuaternion(q);
//	mat *= DirectX::XMLoadFloat4x4(&mRotationMatrix);
//	mat *= XMMatrixScaling(mScale, mScale, mScale);
//	mat *= XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
//	DirectX::XMStoreFloat4x4(&mModelMatrix, mat);
//
//	mModelMatrixBuffer->WriteBuffer256Alignment(&mModelMatrix, sizeof(mModelMatrix), 1);
//}

void FbxModelController::UpdateBundle()
{
	mBundleCmdList->Reset();
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& bundle = mBundleCmdList->GetCommandList();
	bundle->SetDescriptorHeaps(1, mDescHeap->GetDescriptorHeap().GetAddressOf());
	bundle->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	bundle->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	mModel->SetIndexBuffer(bundle);
	mCtrlVertexBuffer->SetBuffer(bundle);

	unsigned int resourceIndex = 0;

	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_LIGHT);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_MATRIX);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_DIFFUSE);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_SPECULAR);
	mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_BUMP);

	unsigned int offsetIndex = 0;
	for (auto& buffer : mAddConstantBuffers)
	{
		mDescHeap->SetGprahicsDescriptorTable(bundle, resourceIndex++, FbxModel::eROOT_PARAMATER_INDEX_MAX + offsetIndex++);
	}
	bundle->DrawIndexedInstanced(static_cast<UINT>(mModel->mIndexes.size()), 1, 0, 0, 0);
	bundle->Close();

	mBundleUpdate = &FbxModelController::NonBundleUpdate;
}

void FbxModelController::NonBundleUpdate()
{
}

void FbxModelController::UpdateDescriptorHeap()
{
	std::vector<std::shared_ptr<Dx12BufferObject>> bufferObj;
	bufferObj.reserve(DEFALUT_RESOURCE_NUM + mAddConstantBuffers.size());
	bufferObj.push_back(mCameraBuffer);
	bufferObj.push_back(mDirLightBuffer);
	bufferObj.push_back(mModelMatrixBuffer);
	auto& texObjects = mModel->GetTextureObjects();
	bufferObj.push_back(texObjects[0]->GetShaderResource());
	bufferObj.push_back(texObjects[1]->GetShaderResource());
	bufferObj.push_back(texObjects[2]->GetShaderResource());
	for (auto& addcbuf : mAddConstantBuffers)
	{
		bufferObj.push_back(addcbuf);
	}
	std::string name = mModel->GetModelName();
	name += "DescriptorHeap";
	mDescHeap.reset(new Dx12DescriptorHeapObject(name, Dx12Ctrl::Instance()->GetDev(), bufferObj, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
}