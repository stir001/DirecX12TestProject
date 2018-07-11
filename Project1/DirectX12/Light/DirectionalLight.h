#pragma once
#include "LightObject.h"

 struct DirectionalLightElement
{
	DirectX::XMFLOAT3 dir;
	DirectX::XMFLOAT4X4 viewProj;
};

class DirectionalLight :
	public LightObject
{
public:
	DirectionalLight(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& dir, float length = 100);
	DirectionalLight(float dirX, float dirY, float dirZ, float length = 100);
	~DirectionalLight();

	//void SetLight(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, int rootparamindex = 2);
private:
	DirectionalLightElement mElement;
};

