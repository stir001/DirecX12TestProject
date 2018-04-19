#include "PMDModel.h"
#include "Dx12Ctrl.h"
#include "ConstantBufferObject.h"
#include "RootSignatureObject.h"
#include "TextureObj.h"
#include "VMDPlayer.h"

#include <algorithm>

PMDModel::PMDModel()
{
}

PMDModel::~PMDModel()
{
	Model::~Model();

}

//void PMDModel::Draw()
//{
//	SetIndexBuffer();
//	SetVertexBuffer();
//	SetBoneBuffer();
//	SetMaterialBuffer();//‚±‚Ì’†‚ÅDraw‚Ü‚Å‚â‚é
//}

void PMDModel::SetMaterialBuffer()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = materialBuffer->GetGPUDescriptorHandle();
	UINT descsize = d12->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	int offset = 0;
	auto cmdList = d12->GetCmdList();
	cmdList->SetPipelineState(d12->GetPiplineState(pso_exitTex));

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = textureDescHeap->GetGPUDescriptorHandleForHeapStart();
	for (unsigned int i = 0; i < materials.size(); i++)
	{
		if (materials[i].texid != -1)
		{
			cmdList->SetPipelineState(d12->GetPiplineState(pso_exitTex));
			if (textureObjects[materials[i].texid] != nullptr)
			{

				cmdList->SetDescriptorHeaps(1, &textureDescHeap);
				textureObjects[materials[i].texid]->SetBuffer();
			}
		}
		else
		{
			cmdList->SetPipelineState(d12->GetPiplineState(pso_notTex));
		}
		materialBuffer->SetDescHeap();
		cmdList->SetGraphicsRootDescriptorTable(rpt_cbv, handle);
		cmdList->DrawIndexedInstanced(materials[i].indexCount, 1, offset, 0, 0);
		handle.ptr += descsize;
		offset += materials[i].indexCount;
	}
}

//void PMDModel::SetMotion(VMDMotion* motion)
//{
//	vmdPlayer->SetVMD(motion);
//}
//
//void PMDModel::PlayMotion(bool loopFlag)
//{
//	vmdPlayer->Play(loopFlag);
//}
//
//void PMDModel::StopMotion()
//{
//	vmdPlayer->Stop();
//}
//
//void PMDModel::SetBoneBuffer()
//{
//	DX12CTRL_INSTANCE
//	boneMatrixBuffer->WriteBuffer(&boneMatrix[0], sizeof(boneMatrix[0]) * boneMatrix.size());
//	boneMatrixBuffer->SetDescHeap();
//	d12->GetCmdList()->SetGraphicsRootDescriptorTable(rpt_bonematrix, boneMatrixBuffer->GetGPUDescriptorHandle());
//}
