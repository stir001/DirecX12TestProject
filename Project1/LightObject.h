#pragma once

#include <DirectXMath.h>

class ConstantBufferObject;

class LightObject
{
protected:
	DirectX::XMFLOAT3 pos;//À•W
	float length;//‹——£ŒÀŠE
	ConstantBufferObject* cbuffer;
public:
	LightObject();
	virtual ~LightObject();
};

