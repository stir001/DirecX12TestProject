#pragma once
#include <DirectXMath.h>

class Rect
{
public:
	Rect();
	Rect(DirectX::XMFLOAT3& inc,float inw,float inh);
	~Rect();

	float GetLeft() const;
	float GetUp() const;
	float GetRight() const;
	float GetDown() const;
	float GetWidth() const;
	float GetHeight() const;
	const DirectX::XMFLOAT3& GetCenter() const;

	void SetCenter(const DirectX::XMFLOAT3& inc);
	void SetWidth(const float inw);
	void SetHeight(const float inh);
private:
	DirectX::XMFLOAT3 mCenter;
	float mWidth;
	float mHeight;
	float mScalse;
};

