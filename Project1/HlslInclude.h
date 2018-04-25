#pragma once
#include <d3dcompiler.h>
#include <string>

class HlslInclude : public ID3DInclude
{
	std::string str;
public:
	HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	HRESULT Close(LPCVOID pData);
	HlslInclude();
	~HlslInclude();
};

