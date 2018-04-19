#include <wpframework.h>
#include <locale.h>

//@param ppBuf 変換後の文字列を入れるバッファ
//@param charWordLen 返還前文字列の長さ
//@param pCharWord 返還前の文字列
//@param ppBufMaxCount 変換後の文字列バッファの最大文字数
//@return 変換した文字列の数
size_t ToWChar(wchar_t** ppBuf, size_t charWordLen, const char* pCharWord, size_t ppBufMaxCount)
{
	setlocale(LC_ALL, "japanese");
	size_t rtn = 0;
	mbstowcs_s(&rtn, *ppBuf, charWordLen, pCharWord, ppBufMaxCount);

	return rtn;
}