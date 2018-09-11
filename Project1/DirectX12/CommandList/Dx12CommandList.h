#pragma once
/**
*	@addtogroup Dx12CommnadList
*	@file Dx12CommandList.h
*	@brief Dx12GraphicsCommnadListをラップしたクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
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
*	@brief ID3D12GraphicsCommnadListをラップしたクラス
*/
class Dx12CommandList
{
public:
	/**
	*	@param	name	コマンドリストの名前
	*	@param	dev		ID3D12Deviceの参照
	*	@param	type	コマンドリストのタイプ
	*/
	Dx12CommandList(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev , D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	virtual ~Dx12CommandList();
	
	/**
	*	@brief	コマンドリストを取得する
	*	@return 保持しているコマンドリスト
	*/
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;

	/**
	*	@brief	コマンドリストのタイプを得る
	*	@return	コマンドリストタイプ
	*/
	D3D12_COMMAND_LIST_TYPE GetType() const;

	/**
	*	@brief	コマンドリストをリセットする
	*	@return	リセットの結果
	*/
	virtual HRESULT Reset() const;

	/**
	*	@brief	コマンドリストを任意のPipelineStateを使ってリセットする
	*	@param[in]	pipelineState	リセット時に使用するpipelinestate
	*	@return	リセットの結果
	*/
	virtual HRESULT Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	/**
	*	@brief	CommnadListに任意のDescriptorHeapをセットする
	*	@param[in]	descHeap	セットするDescriptorHeapをラップしたもの
	*	@return	結果
	*/
	HRESULT SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const;

	/**
	*	@brief	CommnadListに任意のDescriptorHeapをセットする
	*	@param[in]	descHeap	セットするDescriptorHeap
	*	@return	結果
	*/
	HRESULT SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descHeap) const;

	/**
	*	@brief	DescriptorHeapの
	*	@return	結果
	*/
	HRESULT SetGraphicsRootDescriptorTabel(std::shared_ptr<Dx12DescriptorHeapObject>& descHeap, int resourceIndex, int rootpramIndex) const;

	/**
	*	@brief
	*	@return	結果
	*/
	HRESULT SetGraphicsRootDescriptorTable(int rootparamaterIndex, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) const;

	/**
	*	@brief
	*	@return	結果
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

