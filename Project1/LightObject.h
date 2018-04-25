#pragma once

#include <DirectXMath.h>

class ConstantBufferObject;

class LightObject
{
protected:
	DirectX::XMFLOAT3 pos;//ç¿ïW
	float length;//ãóó£å¿äE
	ConstantBufferObject* cbuffer;
public:
	virtual void SetLight() = 0;
	LightObject();
	virtual ~LightObject();
};

