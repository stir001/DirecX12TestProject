#pragma once
#include "DrawObjectController.h"
#include <DirectXMath.h>

class ConstantBufferObject;

class DrawController3D :
	public DrawObjectController
{
public:
	DrawController3D(const std::string& modelName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	virtual ~DrawController3D();

	void SetPositon(const DirectX::XMFLOAT3& pos);
	void SetScale(float scale);
	void AddRotaX(float deg);
	void AddRotaY(float deg);
	void AddRotaZ(float deg);
	void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion);

protected:
	void UpdateMatrix();

	std::shared_ptr<ConstantBufferObject> mModelMatrixBuffer;
	DirectX::XMFLOAT4X4 mModelMatrix;
	float mScale;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT4X4 mRotationMatrix;
	DirectX::XMFLOAT4 mQuaternion;
};

