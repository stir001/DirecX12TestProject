#pragma once
#include <d3dcompiler.h>
#include <string>

class HlslInclude : public ID3DInclude
{
	std::string str;
public:
	HRESULT STDMETHODCALLTYPE Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	HRESULT STDMETHODCALLTYPE Close(LPCVOID pData);
	HlslInclude();
	~HlslInclude();
};

