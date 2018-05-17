#include <wpframework.h>
#include <locale.h>
#include <stdlib.h>

//@param ppBuf �ϊ���̕����������o�b�t�@
//@param charWordLen �ԊґO������̒���
//@param pCharWord �ԊґO�̕�����
//@param ppBufMaxCount �ϊ���̕�����o�b�t�@�̍ő啶����
//@return �ϊ�����������̐�
size_t ToWChar(wchar_t** ppBuf, size_t charWordLen, const char* pCharWord, size_t ppBufMaxCount)
{
	size_t rtn = 0;
	int size = charWordLen;
	*ppBuf = new wchar_t[size + 10];
	mbstowcs_s(&rtn, *ppBuf, charWordLen, pCharWord, _TRUNCATE);

	return rtn;
}