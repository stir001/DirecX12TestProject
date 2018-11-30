#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class SaticPlaneShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	constans	���_����̖@�������ւ̃I�t�Z�b�g
	*	@param[in]	normal		�@��
	*	@param[in]	pos			�����ʒu
	*/
	SaticPlaneShape(float constans
		, const DirectX::XMFLOAT3& normal = DirectX::XMFLOAT3(0.f, 1.f, 0.f)
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	~SaticPlaneShape();
};

