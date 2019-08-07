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

	static bool RayCastTriangle(const DirectX::XMFLOAT2& screenPos,
		const DirectX::XMFLOAT3& pos1,
		const DirectX::XMFLOAT3& pos2,
		const DirectX::XMFLOAT3& pos3);
	
	static DirectX::XMFLOAT3 GetRay(const DirectX::XMFLOAT2& screenPos);

	static DirectX::XMFLOAT3 GetScreenSpace(const DirectX::XMFLOAT2& screenPos, float depth = 0.0f);

	static bool RayCastSphere(const DirectX::XMFLOAT2& screenPos, const DirectX::XMFLOAT3& origin, const float raidus);
private:

	static bool MollerIntersection(const DirectX::XMFLOAT2& screenPos
		,const DirectX::XMFLOAT3& pos0, const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2);
};

