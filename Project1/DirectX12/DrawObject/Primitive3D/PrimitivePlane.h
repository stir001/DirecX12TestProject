#pragma once
#include "PrimitiveObject.h"
class PrimitivePlane :
	public PrimitiveObject
{
public:
	PrimitivePlane();
	PrimitivePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 norm);
	~PrimitivePlane();

	void Draw();
private:
	DirectX::XMFLOAT3 mPos;//íÜêSç¿ïW
	float mLength;
	float mHeight;
	DirectX::XMFLOAT3 mNormal;
};