#include "VertexCustumController.h"
#include "Game/Extend/PrimitiveLine3DRootSignature.h"


VertexCustumController::VertexCustumController() 
	: DrawController3D("CustumVert"
		, Dx12Ctrl::Instance().GetDev()
		, RenderingPassManager::Instance().GetRenderingPassCommandList(DefaultPass::Model))
		, mDrawState(&VertexCustumController::DrawNone)
{
	mBuffer.color = { 1.0f,1.0f,1.0f,1.0f };

	mConstantBuffer = std::make_shared<ConstantBufferObject>("CustumVert",
		Dx12Ctrl::Instance().GetDev(), static_cast<unsigned int>(sizeof(mBuffer)), 1);

	mConstantBuffer->WriteBuffer256Alignment(&mBuffer, sizeof(mBuffer), 1);

	mRootsignature = std::make_shared<PrimitiveLine3DRootSignature>();
	mPipelinestate = std::make_shared<WireFramePipelineState>(mRootsignature, Dx12Ctrl::Instance().GetDev());

	mUpdateDescHeap = &VertexCustumController::UpdateDescHeap;
}


VertexCustumController::~VertexCustumController()
{
}

void VertexCustumController::SetVerts(std::vector<PrimitiveVertex>& verts)
{
	mVerts = std::move(verts);
	mVertexBuffer = std::make_shared<VertexBufferObject>("CustumVert", Dx12Ctrl::Instance().GetDev(),
		sizeof(mVerts.front()), static_cast<unsigned int>(mVerts.size()));
	mVertexBuffer->WriteBuffer(mVerts.data(), static_cast<unsigned int>(sizeof(mVerts.front()) * mVerts.size()));
	mUpdateDescHeap = &VertexCustumController::UpdateDescHeap;
	mDrawState = &VertexCustumController::DrawVertex;
}

void VertexCustumController::SetIndices(std::vector<unsigned int>& indices)
{
	mIndices = std::move(indices);
	mIndexBuffer = std::make_shared<IndexBufferObject>("CustumIndex", Dx12Ctrl::Instance().GetDev(),
		sizeof(mIndices.front()), static_cast<unsigned int>(mIndices.size()));
	mIndexBuffer->WriteBuffer(mIndices.data(), static_cast<unsigned int>(sizeof(mIndices.front()) * mIndices.size()));
	mUpdateDescHeap = &VertexCustumController::UpdateDescHeap;
}


void VertexCustumController::Draw()
{
	(this->*mDrawState)();
}

void VertexCustumController::UpdateDescriptorHeap()
{
	UpdateDescHeap();
}

void VertexCustumController::BundleUpdate()
{
	mBundleCmdList->Reset();
	mBundleCmdList->SetGraphicsRootSignature(mRootsignature);
	mBundleCmdList->SetPipelineState(mPipelinestate);
	mBundleCmdList->SetDescriptorHeap(mDescHeap);
	//mBundleCmdList->IASetIndexBuffer(mIndexBuffer);
	mBundleCmdList->IASetVertexBuffers(&mVertexBuffer,1);
	mBundleCmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	unsigned int resourceIndex = 0U;
	mDescHeap->SetGraphicsDescriptorTable(mBundleCmdList, resourceIndex++, RootParamaterIndex::CAMERA);
	mDescHeap->SetGraphicsDescriptorTable(mBundleCmdList, resourceIndex++, RootParamaterIndex::MATRIX);
	mDescHeap->SetGraphicsDescriptorTable(mBundleCmdList, resourceIndex++, RootParamaterIndex::CBUFFER);

	mBundleCmdList->DrawInstanced(static_cast<unsigned int>(mVerts.size()), 1, 0, 0);
	mBundleCmdList->Close();
}

void VertexCustumController::UpdateDescHeap()
{
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers;
	buffers.push_back(mCameraBuffer);
	buffers.push_back(mModelMatrixBuffer);
	buffers.push_back(mConstantBuffer);

	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>("CustumVertDescHeap",
		Dx12Ctrl::Instance().GetDev(), buffers, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	BundleUpdate();

	mUpdateDescHeap = &VertexCustumController::NonUpdate;
}

void VertexCustumController::NonUpdate()
{
}

void VertexCustumController::DrawVertex()
{
	(this->*mUpdateDescHeap)();
	mCmdList->SetDescriptorHeap(mDescHeap);
	mCmdList->ExecuteBundle(mBundleCmdList);
}

void VertexCustumController::DrawNone()
{
}
