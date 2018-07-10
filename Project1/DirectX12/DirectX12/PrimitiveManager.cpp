#include "PrimitiveManager.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "PrimitivePlane.h"
#include "Dx12Camera.h"
#include "LightObject.h"

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
	mObjects.push_back(new PrimitivePlane(pos, length, height, normal));
}

void PrimitiveManager::Draw()
{
	DX12CTRL_INSTANCE
<<<<<<< HEAD:Project1/DirectX12/DirectX12/PrimitiveManager.cpp
	d12->GetCmdList()->SetPipelineState(d12->GetPipelineState(pso_primitive).Get());
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(rsi_prm).Get());
	//d12->SetCameraBuffer(d12->GetCmdList());
	mLight->SetLight(d12->GetCmdList());
	for (PrimitiveObject* o : mObjects)
=======
	d12.GetCmdList()->SetPipelineState(d12.GetPiplineState(pso_primitive).Get());
	d12.GetCmdList()->SetGraphicsRootSignature(d12.GetRootSignature(rsi_prm).Get());
	d12.SetCameraBuffer();
	light->SetLight();
	for (PrimitiveObject* o : objects)
>>>>>>> f1bde25f671e4006252ea9c7cb50882dbc0ff550:Project1/PrimitiveManager.cpp
	{
		o->Draw();
	}
}

void PrimitiveManager::SetLightObject(std::shared_ptr<LightObject> inlight)
{
	mLight = inlight;
}
