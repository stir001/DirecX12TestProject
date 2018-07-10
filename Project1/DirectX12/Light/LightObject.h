#pragma once

#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

struct ID3D12GraphicsCommandList;
class ConstantBufferObject;

class LightObject
{
public:
	LightObject();
	virtual ~LightObject();

	virtual void SetLight(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, int rootparamindex = 2) = 0;
	std::shared_ptr<ConstantBufferObject>& GetLightBuffer();
protected:
	DirectX::XMFLOAT3 mPos;//ç¿ïW
	float mLength;//ãóó£å¿äE
	std::shared_ptr<ConstantBufferObject> mCbuffer;
};

