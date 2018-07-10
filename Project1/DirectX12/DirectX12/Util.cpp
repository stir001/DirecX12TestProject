#include "Util.h"
#include <DirectXMath.h>

std::string GetRelativePath(const std::string& path)
{
	size_t relative = path.rfind('/') + 1;
	std::string rtn;
	rtn.resize(relative);
	std::copy(path.begin(),path.begin() + relative,rtn.begin());
	return rtn;
}


void StoreMatrixToFloat4x4(DirectX::XMFLOAT4X4& f44, const DirectX::XMMATRIX& mat)
{
	f44._11 = mat.r[0].m128_f32[0]; f44._12 = mat.r[0].m128_f32[1]; f44._13 = mat.r[0].m128_f32[2]; f44._14 = mat.r[0].m128_f32[3];
	f44._21 = mat.r[1].m128_f32[0]; f44._22 = mat.r[1].m128_f32[1]; f44._23 = mat.r[1].m128_f32[2]; f44._24 = mat.r[1].m128_f32[3];
	f44._31 = mat.r[2].m128_f32[0]; f44._32 = mat.r[2].m128_f32[1]; f44._33 = mat.r[2].m128_f32[2]; f44._34 = mat.r[2].m128_f32[3];
	f44._41 = mat.r[3].m128_f32[0]; f44._42 = mat.r[3].m128_f32[1]; f44._43 = mat.r[3].m128_f32[2]; f44._44 = mat.r[3].m128_f32[3];
}