#pragma once
#include <DirectXMath.h>

#ifndef __MXF3OP__
#define __XMF3OP__

//operator

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

void operator+=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

void operator-=(DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lval, float rate);


DirectX::XMFLOAT3 operator*(float rate, const DirectX::XMFLOAT3& rval);

void operator*=(DirectX::XMFLOAT3& lval, float rate);

DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lval, float rate);

void operator/=(DirectX::XMFLOAT3& lval, float rate);

DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& val);

bool operator==(const DirectX::XMFLOAT3& rval, const DirectX::XMFLOAT3& lval);

bool operator!=(const DirectX::XMFLOAT3& rval, const DirectX::XMFLOAT3& lval);

//function

DirectX::XMFLOAT3 NormalizeXMFloat3(const DirectX::XMFLOAT3& val);

float DotXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMFLOAT3 CrossXMFloat3(const DirectX::XMFLOAT3& lval, const DirectX::XMFLOAT3& rval);

DirectX::XMVECTOR CreateQuoternion(DirectX::XMFLOAT3 axis, float deg);

float GetLengthXMFloat3(const DirectX::XMFLOAT3 val);

#endif