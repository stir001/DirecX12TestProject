#pragma once
#include "LightObject.h"

struct DirectionalLightElement
{
	DirectX::XMFLOAT3 dir;
	DirectX::XMMATRIX viewProj;
};

class DirectionalLight :
	public LightObject
{
private:
	DirectionalLightElement element;
public:
	void SetLight();

	DirectionalLight(DirectX::XMFLOAT3& pos,DirectX::XMFLOAT3& dir,float length = 100);
	~DirectionalLight();
};

