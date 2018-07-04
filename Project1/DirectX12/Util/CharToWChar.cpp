#include "CharToWChar.h"

#include <wpframework.h>
#include <locale.h>
#include <stdlib.h>

size_t ToWChar(wchar_t** ppBuf, size_t charWordLen, const char* pCharWord, size_t ppBufMaxCount)
{
	size_t rtn = 0;
	int size = charWordLen;
	*ppBuf = new wchar_t[size + 10];
	mbstowcs_s(&rtn, *ppBuf, charWordLen, pCharWord, _TRUNCATE);

	return rtn;
}

size_t ToWChar(wchar_t** ppBuf, std::string cstr)
{
	size_t rtn = 0;
	cstr.push_back('\0');
	*ppBuf = new wchar_t[cstr.size()];
	mbstowcs_s(&rtn, *ppBuf, cstr.size(), cstr.data(), _TRUNCATE);

	return rtn;
}

size_t ToChar(char** ppBuf, size_t ppBufSizeInByte, const wchar_t* wcStr, size_t convertWordNum)
{
	size_t rtn = 0;
	int size = convertWordNum;
	*ppBuf = new char[size];
	wcstombs_s(&rtn, *ppBuf, ppBufSizeInByte, wcStr, _TRUNCATE);

	return rtn;
}