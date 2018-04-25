#include "HlslInclude.h"
#include <iostream>
#include <string>


HlslInclude::HlslInclude()
{
}


HlslInclude::~HlslInclude()
{
}

HRESULT HlslInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	FILE* fp;
	fopen_s(&fp, pFileName, "r");
	if (fp == nullptr) return E_FAIL;

	str.clear();
	UINT bytes = 0;
	char data = 0;
	int check = -1;
	while (true)
	{
		check = fread(&data, sizeof(data), 1, fp);
		if (check == 0) break;
		str.push_back(data);
		bytes += sizeof(data);
	}

	fclose(fp);
	
	*ppData = static_cast<LPCVOID>(str.data());
	*pBytes = bytes;

	return S_OK;
}

HRESULT HlslInclude::Close(LPCVOID pData)
{
	return S_OK;
}
