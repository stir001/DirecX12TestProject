#pragma once
#include <DirectXMath.h>

namespace PRIMITIVE {
	struct Triangle;
}

class PlaneCollisionCheckerXZ
{
public:
	PlaneCollisionCheckerXZ();
	~PlaneCollisionCheckerXZ();

	bool IsInTriangle(const DirectX::XMFLOAT3& point, const PRIMITIVE::Triangle& triangle) const;
private:
	float CrossXZ(const DirectX::XMFLOAT3& val1, const DirectX::XMFLOAT3& val2) const;
	int Sign(float value) const;
};

