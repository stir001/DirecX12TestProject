#pragma once
#include <wrl.h>
struct ID3D12CommandQueue;

class RenderingPathObject
{
public:
	RenderingPathObject();
	virtual ~RenderingPathObject();

	virtual void FirstUpdate() = 0;	//�e�t���[���̈�ԍŏ��Ɏ��s�����
	virtual void LastUpdate() = 0;	//���O��RenderingPathObject��ExcutePath�����s���ꂽ���ƂɎ��s�����
	virtual void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue) = 0;	//�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	virtual void ResetCommandList() = 0;	//ExcutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	virtual void SetActive(bool isActive);	//���̃p�X��L���ɂ��邩�����ɂ��邩�̐ݒ���s��
	virtual bool IsActive() const;			//���̃p�X���L������������Ԃ�:true �L��, false ����
protected:
private:
	bool mIsActive;
};

