#pragma once
#include <DirectXMath.h>

class TransformMatrixCalculator
{
public:
	TransformMatrixCalculator();
	~TransformMatrixCalculator();

	virtual void SetPositon(const DirectX::XMFLOAT3& pos) = 0;
	virtual void SetScale(DirectX::XMFLOAT3& pos) = 0;
	virtual void AddRotaX(float deg) = 0;
	virtual void AddRotaY(float deg) = 0;
	virtual void AddRotaZ(float deg) = 0;
	virtual void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion) = 0;
	virtual DirectX::XMFLOAT4X4 GetAMatrix() const = 0;
};

