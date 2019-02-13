#pragma once
#include "Base/RenderingPassObject.h"
#include <memory>
#include <wrl.h>

class Dx12BufferObject;
class LightObject;
class Dx12DescriptorHeapObject;
struct ID3D12Device;

class ShadowmapPass :
	public RenderingPassObject
{
public:
	ShadowmapPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~ShadowmapPass();

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

	/**
	*	@brief	�V���h�E�}�b�v�e�N�X�`�����擾����
	*	@return	shadowmap
	*/
	std::shared_ptr<Dx12BufferObject> GetShadowmap();
private:
	std::shared_ptr<Dx12BufferObject> mShadowmapDepth;
	std::shared_ptr<LightObject> mDirectionalLight;
	std::shared_ptr<Dx12DescriptorHeapObject> mShadowDepthDescHeap;
	float mBufferSize;
};
