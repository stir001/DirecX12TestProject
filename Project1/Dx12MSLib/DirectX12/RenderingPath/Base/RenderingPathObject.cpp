#include "stdafx.h"
#include "RenderingPathObject.h"
#include <d3d12.h>

RenderingPathObject::RenderingPathObject()
{
}

RenderingPathObject::~RenderingPathObject()
{
}

void RenderingPathObject::SetActive(bool isActive)
{
	mIsActive = isActive;
}

bool RenderingPathObject::IsActive() const
{
	return mIsActive;
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderingPathObject::GetRenderTarget()
{
	return nullptr;
}
