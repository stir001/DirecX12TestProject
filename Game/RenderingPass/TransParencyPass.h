#pragma once
#include <Dx12MSLib.h>

class TransParencyPass : RenderingPassObject
{
public:
	TransParencyPass(std::shared_ptr<Dx12DescriptorHeapObject>& rtvHeap,
		std::shared_ptr<Dx12DescriptorHeapObject>& depthHeap);
	~TransParencyPass();

	/**
	*	�e�t���[���̈�ԍŏ��Ɏ��s�����
	*/
	void FirstUpdate();

	/**
	*	���̃p�X��ExecutePath���Ă΂�钼�O�Ɏ��s�����
	*	��{�I�ɂ����ŃR�}���h���X�g��close���Ă�
	*/
	void PreExecuteUpdate();

	/**
	*	�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	void ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExecutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	void ResetCommandList();

	/**
	*	�R�}���h���X�g���擾����
	*/
	std::shared_ptr<Dx12CommandList> GetCommandList();
private:
	std::shared_ptr<Dx12DescriptorHeapObject> mRTVDescHeap;
	std::shared_ptr<Dx12DescriptorHeapObject> mDepthHeap;
};

