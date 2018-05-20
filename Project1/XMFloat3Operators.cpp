#include "XMFloat3Operators.h"

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return DirectX::XMFLOAT3(lval.x + rval.x, lval.y + rval.y, lval.z + rval.z);
}

void operator+=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	lval = lval + rval;
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return DirectX::XMFLOAT3(lval.x - rval.x, lval.y - rval.y, lval.z - rval.z);
}

void operator-=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	lval = lval - rval;
}

DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lval, float rato)
{
	return DirectX::XMFLOAT3(lval.x * rato, lval.y * rato, lval.z * rato);
}

DirectX::XMFLOAT3 operator*(float rato, const DirectX::XMFLOAT3& rval)
{
	return rval * rato;
}

void operator*=(DirectX::XMFLOAT3& lval, float rato)
{
	lval = lval * rato;
}

DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lval, float rato)
{
	return DirectX::XMFLOAT3(lval.x / rato, lval.y / rato, lval.z / rato);
}

void operator/=(DirectX::XMFLOAT3& lval, float rato)
{
	lval = lval / rato;
}

DirectX::XMFLOAT3 NormalizeXMFloat3(const DirectX::XMFLOAT3& val)
{
	float len = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);
	return DirectX::XMFLOAT3(val.x / len, val.y / len, val.z / len);
}

float DotXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return lval.x * rval.x + lval.y * rval.y + lval.z * rval.z;
}

DirectX::XMFLOAT3 CrossXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return  NormalizeXMFloat3(DirectX::XMFLOAT3(lval.y * rval.z - lval.z * rval.y, lval.z * rval.x - lval.x * rval.z, lval.x * rval.y - lval.y * rval.x));
}

DirectX::XMVECTOR CreateQuoternion(DirectX::XMFLOAT3 axis, float deg)
{
	axis = NormalizeXMFloat3(axis);
	return DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(axis.x * sinf(DirectX::XMConvertToRadians(deg / 2.0f)), axis.y * sinf(DirectX::XMConvertToRadians(deg / 2.0f)), axis.z * sinf(DirectX::XMConvertToRadians(deg / 2.0f)), cosf(DirectX::XMConvertToRadians(deg / 2.0f))));
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val)
{
	return -1.0f * val;
}

bool operator==(const DirectX::XMFLOAT3& rval, const DirectX::XMFLOAT3& lval)
{
	return (rval.x == lval.x && rval.y == lval.y && rval.z == lval.z);
}

bool operator!=(const DirectX::XMFLOAT3& rval, const DirectX::XMFLOAT3& lval)
{
	return !(rval == lval);
}