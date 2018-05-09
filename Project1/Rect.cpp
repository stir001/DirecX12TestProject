#include "Rect.h"



Rect::Rect():mCenter(0,0,0),mWidth(10),mHeight(10)
{
}

Rect::Rect(DirectX::XMFLOAT3& inc, float inw, float inh):mCenter(inc),mWidth(inw),mHeight(inh)
{
}

Rect::~Rect()
{
}

float Rect::GetLeft() const
{
	return mCenter.x - mWidth / 2.0f;
}

float Rect::GetUp() const
{
	return mCenter.y + mHeight / 2.0f;
}

float Rect::GetRight() const
{
	return mCenter.x + mWidth / 2.0f;
}

float Rect::GetDown() const
{
	return mCenter.y - mHeight / 2.0f;
}

float Rect::GetWidth() const
{
	return mWidth;
}

float Rect::GetHeight() const
{
	return mHeight;
}

const DirectX::XMFLOAT3& Rect::GetCenter() const
{
	return mCenter;
}

void Rect::SetCenter(const DirectX::XMFLOAT3& inc)
{
	mCenter = inc;
}

void Rect::SetWidth(const float inw)
{
	mWidth = inw;
}

void Rect::SetHeight(const float inh)
{
	mHeight = inh;
}
