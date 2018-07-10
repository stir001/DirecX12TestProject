#include "XMFloatOperators.h"

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
	float len = sqrtf(val.x * val.x + val.y * val.y + val.z * val.z);
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

DirectX::XMVECTOR CreateQuoternion(const DirectX::XMFLOAT3& axis, float deg)
{
	DirectX::XMFLOAT3 rtn = NormalizeXMFloat3(axis);
	return DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(
		rtn.x * sinf(DirectX::XMConvertToRadians(deg / 2.0f)), 
		rtn.y * sinf(DirectX::XMConvertToRadians(deg / 2.0f)), 
		rtn.z * sinf(DirectX::XMConvertToRadians(deg / 2.0f)), 
		cosf(DirectX::XMConvertToRadians(deg / 2.0f))));
}

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val)
{
	return -1.0f * val;
}

bool operator==(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return (lval.x == rval.x && lval.y == rval.y && lval.z == rval.z);
}

bool operator!=(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval)
{
	return !(lval == rval);
}

void operator+=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT3& rval)
{
	lval.x += rval.x; lval.y += rval.y; lval.z += rval.z;
}

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const float rval)
{
	return DirectX::XMFLOAT4X4(
		lval._11 * rval, lval._12 * rval, lval._13 * rval, lval._14 * rval,
		lval._21 * rval, lval._22 * rval, lval._23 * rval, lval._24 * rval,
		lval._31 * rval, lval._32 * rval, lval._33 * rval, lval._34 * rval,
		lval._41 * rval, lval._42 * rval, lval._43 * rval, lval._44 * rval);
}

DirectX::XMFLOAT4X4 operator*(const float lval, const DirectX::XMFLOAT4X4& rval)
{
	return rval * lval;
}

DirectX::XMFLOAT4X4 operator+(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4X4(
		rval._11 + lval._11, rval._12 + lval._12, rval._13 + lval._13, rval._14 + lval._14,
		rval._21 + lval._21, rval._22 + lval._22, rval._23 + lval._23, rval._24 + lval._24,
		rval._31 + lval._31, rval._32 + lval._32, rval._33 + lval._33, rval._34 + lval._34,
		rval._41 + lval._41, rval._42 + lval._42, rval._43 + lval._43, rval._44 + lval._44);
}

void operator+=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	lval = lval + rval;
}

DirectX::XMFLOAT4X4 operator-(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4X4(
		rval._11 - lval._11, rval._12 - lval._12, rval._13 - lval._13, rval._14 - lval._14,
		rval._21 - lval._21, rval._22 - lval._22, rval._23 - lval._23, rval._24 - lval._24,
		rval._31 - lval._31, rval._32 - lval._32, rval._33 - lval._33, rval._34 - lval._34,
		rval._41 - lval._41, rval._42 - lval._42, rval._43 - lval._43, rval._44 - lval._44);
}

void operator-=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval)
{
	lval = lval - rval;
}

DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4(
		lval.x * rval._11 + lval.y * rval._21 + lval.z * rval._31 + lval.w * rval._41,
		lval.x * rval._12 + lval.y * rval._22 + lval.z * rval._32 + lval.w * rval._42,
		lval.x * rval._13 + lval.y * rval._23 + lval.z * rval._33 + lval.w * rval._43,
		lval.x * rval._14 + lval.y * rval._24 + lval.z * rval._34 + lval.w * rval._44);
}

void operator*=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval)
{
	lval = lval * rval;
}

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, DirectX::XMFLOAT4X4& rval)
{
	return DirectX::XMFLOAT4X4(
		(lval._11 * rval._11) + (lval._12 * rval._21) + (lval._13 * rval._31) + (lval._14 * rval._41),
		(lval._11 * rval._12) + (lval._12 * rval._22) + (lval._13 * rval._32) + (lval._14 * rval._42),
		(lval._11 * rval._13) + (lval._12 * rval._23) + (lval._13 * rval._33) + (lval._14 * rval._43),
		(lval._11 * rval._14) + (lval._12 * rval._24) + (lval._13 * rval._34) + (lval._14 * rval._44),

		(lval._21 * rval._11) + (lval._22 * rval._21) + (lval._23 * rval._31) + (lval._24 * rval._41),
		(lval._21 * rval._12) + (lval._22 * rval._22) + (lval._23 * rval._32) + (lval._24 * rval._42),
		(lval._21 * rval._13) + (lval._22 * rval._23) + (lval._23 * rval._33) + (lval._24 * rval._43),
		(lval._21 * rval._14) + (lval._22 * rval._24) + (lval._23 * rval._34) + (lval._24 * rval._44),

		(lval._31 * rval._11) + (lval._32 * rval._21) + (lval._33 * rval._31) + (lval._34 * rval._41),
		(lval._31 * rval._12) + (lval._32 * rval._22) + (lval._33 * rval._32) + (lval._34 * rval._42),
		(lval._31 * rval._13) + (lval._32 * rval._23) + (lval._33 * rval._33) + (lval._34 * rval._43),
		(lval._31 * rval._14) + (lval._32 * rval._24) + (lval._33 * rval._34) + (lval._34 * rval._44),

		(lval._41 * rval._11) + (lval._42 * rval._21) + (lval._43 * rval._31) + (lval._44 * rval._41),
		(lval._41 * rval._12) + (lval._42 * rval._22) + (lval._43 * rval._32) + (lval._44 * rval._42),
		(lval._41 * rval._13) + (lval._42 * rval._23) + (lval._43 * rval._33) + (lval._44 * rval._43),
		(lval._41 * rval._14) + (lval._42 * rval._24) + (lval._43 * rval._34) + (lval._44 * rval._44)
		);
}

void operator*=(DirectX::XMFLOAT4X4& lval, DirectX::XMFLOAT4X4& rval)
{
	lval = lval * rval;
}