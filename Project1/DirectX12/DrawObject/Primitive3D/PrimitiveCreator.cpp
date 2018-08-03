#include "PrimitiveCreator.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "PrimitivePlane.h"
#include "Dx12Camera.h"
#include "LightObject.h"
#include "PrimitiveController.h"
#include "RenderingPathManager.h"
#include "PrimitiveRootSignature.h"
#include "PrimitivePipelineState.h"
#include "DirectionalLight.h"
#include "PrimitiveCube.h"
#include "TextureLoader.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <tchar.h>

PrimitiveCreator::PrimitiveCreator():mRootsiganture(new PrimitiveRootSignature(Dx12Ctrl::Instance().GetDev()))
	,mPipelineState(new PrimitivePipelineState(mRootsiganture, Dx12Ctrl::Instance().GetDev()))
	,mLight(new DirectionalLight(1,-1,1)),mCommnadList(RenderingPathManager::Instance().GetRenderingPathCommandList(0))
{
}


PrimitiveCreator::~PrimitiveCreator()
{
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreatePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 normal)
{
	return CreateController(std::shared_ptr<PrimitivePlane>(new PrimitivePlane(pos, length, height, normal)));
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateCube(float length, const std::string& texPath)
{
	auto tex = TextureLoader::Instance().LoadTexture(texPath);
	std::shared_ptr<PrimitiveController> rtn = CreateController(std::shared_ptr<PrimitiveCube>(new PrimitiveCube(length)));
	rtn->SetTexture(tex);
	return rtn;
}

void PrimitiveCreator::SetParamaters(std::shared_ptr<PrimitiveController>& ctrl)
{
	ctrl->SetRootSignature(mRootsiganture);
	ctrl->SetPipelineState(mPipelineState);
	ctrl->SetLightBuffer(mLight);
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateController(const std::shared_ptr<PrimitiveObject>& primitive)
{
	std::shared_ptr<PrimitiveController> rtn(new PrimitiveController(primitive, Dx12Ctrl::Instance().GetDev(), mCommnadList, Dx12Ctrl::Instance().GetCamera()));
	SetParamaters(rtn);
	return rtn;
}

void PrimitiveCreator::SetLightObject(std::shared_ptr<LightObject> inlight)
{
	mLight = inlight;
}
