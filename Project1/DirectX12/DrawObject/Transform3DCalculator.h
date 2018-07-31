#pragma once
#include "TransformMatrixCalculator.h"

class Transform3DCalculator : public TransformMatrixCalculator
{
public:
	Transform3DCalculator();
	~Transform3DCalculator();

	void SetPositon(const DirectX::XMFLOAT3& pos);
	void SetScale(DirectX::XMFLOAT3& scale);
	void AddRotaX(float deg);
	void AddRotaY(float deg);
	void AddRotaZ(float deg);
	void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion);
	DirectX::XMFLOAT4X4 GetAMatrix();
private:
	void UpdateMatrix();
	void NonUpdateMatrix();

	void (Transform3DCalculator::*mMatrixUpdate)();
	DirectX::XMFLOAT4X4 mAMatrix;
	DirectX::XMFLOAT4X4 mRotaMatrix;
	DirectX::XMFLOAT4 mPos;
	DirectX::XMFLOAT4 mScale;
	DirectX::XMFLOAT4 mQuaternion;
};

