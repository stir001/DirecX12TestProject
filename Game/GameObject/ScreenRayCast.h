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
		*	Šî€“_‚©‚ç“–‚½‚Á‚½êŠ‚Ö‚Ì’¼ü‹——£
		*/
		float length;

		/**
		*	“–‚½‚Á‚½À•W
		*/
		DirectX::XMFLOAT3 hitPos;
	};

	static Hit RayCastPlane(const DirectX::XMFLOAT2& screenPos,
			const DirectX::XMFLOAT3& origin,
			const DirectX::XMFLOAT3& normal);
};

