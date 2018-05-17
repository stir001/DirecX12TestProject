#include <wpframework.h>
#include <locale.h>
#include <stdlib.h>

//@param ppBuf 変換後の文字列を入れるバッファ
//@param charWordLen 返還前文字列の長さ
//@param pCharWord 返還前の文字列
//@param ppBufMaxCount 変換後の文字列バッファの最大文字数
//@return 変換した文字列の数
size_t ToWChar(wchar_t** ppBuf, size_t charWordLen, const char* pCharWord, size_t ppBufMaxCount)
{
	size_t rtn = 0;
	int size = charWordLen;
	*ppBuf = new wchar_t[size + 10];
	mbstowcs_s(&rtn, *ppBuf, charWordLen, pCharWord, _TRUNCATE);

	return rtn;
}