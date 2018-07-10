#pragma once
#ifndef __MXF3OP__
#define __XMF3OP__
#include <DirectXMath.h>

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

void operator+=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

void operator-=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lval, float rato);


DirectX::XMFLOAT3 operator*(float rato, const DirectX::XMFLOAT3& rval);

void operator*=(DirectX::XMFLOAT3& lval, float rato);

DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lval, float rato);

void operator/=(DirectX::XMFLOAT3& lval, float rato);

DirectX::XMFLOAT3 NormalizeXMFloat3(const DirectX::XMFLOAT3& val);

float DotXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 CrossXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMVECTOR CreateQuoternion(const DirectX::XMFLOAT3& axis, float deg);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val);

bool operator==(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

bool operator!=(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

void operator+=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, const float rval);

DirectX::XMFLOAT4X4 operator*( const float lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator+(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

void operator+=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator-(const DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

void operator-=(DirectX::XMFLOAT4X4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval);

void operator*=(DirectX::XMFLOAT4& lval, const DirectX::XMFLOAT4X4& rval);

DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lval, DirectX::XMFLOAT4X4& rval);

void operator*=(DirectX::XMFLOAT4X4& lval, DirectX::XMFLOAT4X4& rval);
#endif