#pragma once
/**
*	@addtogroup Dx12CommnadList
*	@file Dx12CommandList.h
*	@brief Dx12GraphicsCommnadList�����b�v�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>

class Dx12BufferObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup Dx12CommnadList
*	@class Dx12CommnadList
*	@brief ID3D12GraphicsCommnadList�����b�v�����N���X
*/
class Dx12CommandList
{
public:
	/**
	*	@param	name	�R�}���h���X�g�̖��O
	*	@param	dev		ID3D12Device�̎Q��
	*	@param	type	�R�}���h���X�g�̃^�C�v
	*/
	Dx12CommandList(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev , D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	virtual ~Dx12CommandList();
	
	/**
	*	@brief	�R�}���h���X�g���擾����
	*	@return �ێ����Ă���R�}���h���X�g
	*/
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;

	/**
	*	@brief	�R�}���h���X�g�̃^�C�v�𓾂�
	*	@return	�R�}���h���X�g�^�C�v
	*/
	D3D12_COMMAND_LIST_TYPE GetType() const;

	/**
	*	@brief	�R�}���h���X�g�����Z�b�g����
	*	@return	���Z�b�g�̌���
	*/
	virtual HRESULT Reset() const;

	/**
	*	@brief	�R�}���h���X�g��C�ӂ�PipelineState���g���ă��Z�b�g����
	*	@param[in]	pipelineState	���Z�b�g���Ɏg�p����pipelinestate
	*	@return	���Z�b�g�̌���
	*/
	virtual HRESULT Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	/**
	*	@brief	CommnadList�ɔC�ӂ�DescriptorHeap���Z�b�g����
	*	@param[in]	descHeap	�Z�b�g����DescriptorHeap�����b�v��������
	*	@return	����
	*/
	HRESULT SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const;

	/**
	*	@brief	CommnadList�ɔC�ӂ�DescriptorHeap���Z�b�g����
	*	@param[in]	descHeap	�Z�b�g����DescriptorHeap
	*	@return	����
	*/
	HRESULT SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descHeap) const;

	/**
	*	@brief	DescriptorHeap��
	*	@return	����
	*/
	HRESULT SetGraphicsRootDescriptorTabel(std::shared_ptr<Dx12DescriptorHeapObject>& descHeap, int resourceIndex, int rootpramIndex) const;

	/**
	*	@brief
	*	@return	����
	*/
	HRESULT SetGraphicsRootDescriptorTable(int rootparamaterIndex, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) const;

	/**
	*	@brief
	*	@return	����
	*/
	HRESULT TransitionBarrier(std::shared_ptr<Dx12BufferObject>& resource, D3D12_RESOURCE_STATES transitionState) const;
	
	void ClearDepthView(D3D12_CPU_DESCRIPTOR_HANDLE depthCpuHandle, float clearValue ,D3D12_RECT* rect, unsigned int rectNum = 0);
	void ClearStencilView(D3D12_CPU_DESCRIPTOR_HANDLE stencilCpuHandle, unsigned int clearValue, D3D12_RECT* rect, unsigned int rectNum = 0);
	void OMSetRenderTargets(unsigned int renderTargetCount, D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE* depthCpuHandle = nullptr);
	void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, const float colorRGBA[4], D3D12_RECT* rect, unsigned int rectNum = 0);
	void RSSetViewports(D3D12_VIEWPORT* viewPorts, unsigned int viewportCount = 1);
	void RSSetScissorRects(D3D12_RECT* scissorRects, unsigned int scissorRectsCount = 1);
	const std::string& GetName() const;

	void Close() const;
private:
	std::string mName;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdallcator;
	const D3D12_COMMAND_LIST_TYPE mType;

	HRESULT GetDeviceRemoveReason() const;
};

