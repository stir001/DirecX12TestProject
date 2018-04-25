#pragma once

#include <DirectXMath.h>

class ConstantBufferObject;

class LightObject
{
protected:
	DirectX::XMFLOAT3 pos;//���W
	float length;//�������E
	ConstantBufferObject* cbuffer;
public:
	virtual void SetLight() = 0;
	LightObject();
	virtual ~LightObject();
};

