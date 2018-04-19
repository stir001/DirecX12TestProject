#include "PrimitiveManager.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "PrimitivePlane.h"
#include "Dx12Camera.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <tchar.h>

PrimitiveManager::PrimitiveManager()
{
}


PrimitiveManager::~PrimitiveManager()
{
}

void PrimitiveManager::CreatePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 normal)
{
	objects.push_back(new PrimitivePlane(pos, length, height, normal));
}

void PrimitiveManager::Draw()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pso_primitive));
	d12->GetCamera()->SetBuffer();
	for (PrimitiveObject* o : objects)
	{
		o->Draw();
	}
}
