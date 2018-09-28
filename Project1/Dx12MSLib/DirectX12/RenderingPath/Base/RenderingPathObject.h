#pragma once
#include <wrl.h>
struct ID3D12CommandQueue;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

class RenderingPathObject
{
public:
	RenderingPathObject();
	virtual ~RenderingPathObject();

	/**
	*�e�t���[���̈�ԍŏ��Ɏ��s�����
	*/
	virtual void FirstUpdate() = 0;	

	/**
	*���O��RenderingPathObject��ExcutePath�����s���ꂽ���ƂɎ��s�����
	*/
	virtual void LastUpdate() = 0;

	/**
	*�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	virtual void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue) = 0;

	/**
	*ExcutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	virtual void ResetCommandList() = 0;

	/**
	*�R�}���h���X�g���擾����
	*/
	virtual Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommnadList() = 0;

	/**
	*�ŏI�����_�����O���ʂ�Ԃ��֐��@��ԍŌ�̃p�X�̂ݕK�{
	*/
	virtual Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTarget();

	/**
	*���̃p�X��L���ɂ��邩�����ɂ��邩�̐ݒ���s��
	*/
	virtual void SetActive(bool isActive);

	/**
	*���̃p�X���L������������Ԃ�:true �L��, false ����
	*/
	virtual bool IsActive() const;
protected:
private:
	bool mIsActive;
};

