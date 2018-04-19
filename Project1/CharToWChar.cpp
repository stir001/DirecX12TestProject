#include <wpframework.h>
#include <locale.h>

//@param ppBuf �ϊ���̕����������o�b�t�@
//@param charWordLen �ԊґO������̒���
//@param pCharWord �ԊґO�̕�����
//@param ppBufMaxCount �ϊ���̕�����o�b�t�@�̍ő啶����
//@return �ϊ�����������̐�
size_t ToWChar(wchar_t** ppBuf, size_t charWordLen, const char* pCharWord, size_t ppBufMaxCount)
{
	setlocale(LC_ALL, "japanese");
	size_t rtn = 0;
	mbstowcs_s(&rtn, *ppBuf, charWordLen, pCharWord, ppBufMaxCount);

	return rtn;
}