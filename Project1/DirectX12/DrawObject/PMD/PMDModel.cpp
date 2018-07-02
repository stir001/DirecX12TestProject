#include "PMDModel.h"
#include "Dx12Ctrl.h"
#include "ConstantBufferObject.h"
#include "RootSignatureObject.h"
#include "TextureObject.h"
#include "VMDPlayer.h"

#include <algorithm>

PMDModel::PMDModel()
{
}

PMDModel::~PMDModel()
{
	Model::~Model();

}

void PMDModel::SetMaterialBuffer()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = {};// = mMaterialBuffer->GetGPUViewHandle();
	UINT descsize = d12->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	int offset = 0;
	auto cmdList = d12->GetCmdList();
	cmdList->SetPipelineState(d12->GetPipelineState(pso_exitTex).Get());

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {};// mTextureDescHeap->GetGPUDescriptorHandleForHeapStart();
	for (unsigned int i = 0; i < mMaterials.size(); i++)
	{
		if (mMaterials[i].texid != -1)
		{
			cmdList->SetPipelineState(d12->GetPipelineState(pso_exitTex).Get());
			if (mTextureObjectects[mMaterials[i].texid] != nullptr)
			{
				//cmdList->SetDescriptorHeaps(1, mTextureDescHeap.GetAddressOf());
				//mTextureObjectects[mMaterials[i].texid]->SetBuffer(d12->GetCmdList(), eROOT_PARAMATER_INDEX_TEXTURE);
			}
		}
		else
		{
			cmdList->SetPipelineState(d12->GetPipelineState(pso_notTex).Get());
		}
		//mMaterialBuffer->SetDescHeap(d12->GetCmdList());
		cmdList->SetGraphicsRootDescriptorTable(eROOT_PARAMATER_INDEX_MATERIAL, handle);
		cmdList->DrawIndexedInstanced(mMaterials[i].indexCount, 1, offset, 0, 0);
		handle.ptr += descsize;
		offset += mMaterials[i].indexCount;
	}
}

