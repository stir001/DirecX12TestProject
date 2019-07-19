#pragma once
#include <DirectXMath.h>

class ScreenRayCast
{
public:
	ScreenRayCast();
	~ScreenRayCast();

	struct Hit
	{
		/**
		*	基準点から当たった場所への直線距離
		*/
		float length;

		/**
		*	当たった座標
		*/
		DirectX::XMFLOAT3 hitPos;
	};

	static Hit RayCastPlane(const DirectX::XMFLOAT2& screenPos,
			const DirectX::XMFLOAT3& origin,
			const DirectX::XMFLOAT3& normal);
};

