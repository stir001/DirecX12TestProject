#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class SaticPlaneShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	constans	原点からの法線方向へのオフセット
	*	@param[in]	normal		法線
	*	@param[in]	pos			初期位置
	*/
	SaticPlaneShape(float constans
		, const DirectX::XMFLOAT3& normal = DirectX::XMFLOAT3(0.f, 1.f, 0.f)
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~SaticPlaneShape();
};

