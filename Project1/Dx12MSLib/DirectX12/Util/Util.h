#pragma once
#include <string>

namespace DirectX
{
	struct XMFLOAT4X4;
	struct XMMATRIX;
}

std::string GetRelativePath(const std::string& path);

void StoreMatrixToFloat4x4(DirectX::XMFLOAT4X4& f44, const DirectX::XMMATRIX& mat);