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
		*	��_���瓖�������ꏊ�ւ̒�������
		*/
		float length;

		/**
		*	�����������W
		*/
		DirectX::XMFLOAT3 hitPos;
	};

	static Hit RayCastPlane(const DirectX::XMFLOAT2& screenPos,
			const DirectX::XMFLOAT3& origin,
			const DirectX::XMFLOAT3& normal);
};

