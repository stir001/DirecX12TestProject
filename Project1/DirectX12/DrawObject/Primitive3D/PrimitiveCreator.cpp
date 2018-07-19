#include "PrimitiveCreator.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "PrimitivePlane.h"
#include "Dx12Camera.h"
#include "LightObject.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <tchar.h>

PrimitiveCreator::PrimitiveCreator()
{
}


PrimitiveCreator::~PrimitiveCreator()
{
}

std::shared_ptr<PrimitivePlane> PrimitiveCreator::CreatePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 normal)
{
	return std::shared_ptr<PrimitivePlane>(new PrimitivePlane(pos, length, height, normal));
}

void PrimitiveCreator::SetLightObject(std::shared_ptr<LightObject> inlight)
{
	mLight = inlight;
}
