#include "FbxModel.h"
#include "TextureObj.h"
#include "Dx12Ctrl.h"

using namespace Fbx;

FbxModel::FbxModel()
{

}

FbxModel::~FbxModel()
{

}

void FbxModel::SetTexture()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetDescriptorHeaps(1, &textureDescHeap);
	textureObjects[0]->SetBuffer(5);
	textureObjects[1]->SetBuffer(4);
	textureObjects[2]->SetBuffer(0);
	textureObjects[3]->SetBuffer(6);
}