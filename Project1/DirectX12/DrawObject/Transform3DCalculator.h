#pragma once
#include "TransformMatrixCalculator.h"

class Transform3DCalculator : public TransformMatrixCalculator
{
public:
	Transform3DCalculator();
	~Transform3DCalculator();

	void AddPositon(const DirectX::XMFLOAT3& pos);
	void AddScale(const DirectX::XMFLOAT3& scale);
	void AddRotaX(float deg);
	void AddRotaY(float deg);
	void AddRotaZ(float deg);
	void AddRotaQuaternion(const DirectX::XMFLOAT4& quaternion);
	DirectX::XMFLOAT4X4 GetAMatrix();
	void Init();
private:
	DirectX::XMFLOAT4X4 Multipli(DirectX::XMFLOAT4X4& matF44, DirectX::XMMATRIX& matXM);

	DirectX::XMFLOAT4X4 mAMatrix;
};

