#pragma once
#include "IPrimitive2D.h"

class Primitive2DLine : public IPrimitive2D
{
public:
	Primitive2DLine(const DirectX::XMFLOAT3& point1,const DirectX::XMFLOAT3& point2);
	~Primitive2DLine();

	void Draw();
	void SetLength(float length);
	void SetRota(float deg);
private:
	DirectX::XMFLOAT3 mColor;
	float mRad;
	float mLength;

	void UpdateBuffer();
};

