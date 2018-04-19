#pragma once
#include "PrimitiveObject.h"
class PrimitivePlane :
	public PrimitiveObject
{
private:
	DirectX::XMFLOAT3 pos;//íÜêSç¿ïW
	float length;
	float height;
	DirectX::XMFLOAT3 normal;
public:
	void Draw();
	PrimitivePlane();
	PrimitivePlane(DirectX::XMFLOAT3 pos,float length,float height,DirectX::XMFLOAT3 norm);
	~PrimitivePlane();
};