#pragma once
#include <d3d12.h>
#include "d3dx12.h"
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>

class Dx12BufferViewDesc;

enum eDX12_DESC_TYPE
{
	eDX12_DESC_TYPE_CBV,
	eDX12_DESC_TYPE_SRV,
	eDX12_DESC_TYPE_UAV,
	eDX12_DESC_TYPE_RTV,
	eDX12_DESC_TYPE_DSV,
};

struct BufferViewInfo
{
	eDX12_DESC_TYPE type;
	unsigned int elementCount;
	union
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvViewDesc;
		D3D12_SHADER_RESOURCE_VIEW_DESC srvViewDesc;
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavViewDesc;
		D3D12_RENDER_TARGET_VIEW_DESC rtvViewDesc;
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvViewDesc;
	};
};

class Dx12BufferObject
{
public:
	Dx12BufferObject(const std::string& name);
	Dx12BufferObject(Microsoft::WRL::ComPtr<ID3D12Resource>& rsc, const std::string& name,
		unsigned int elemenetSize, unsigned int elementCount);
	virtual ~Dx12BufferObject();

	/**
	*@fn
	*@param pData �������ރf�[�^�̃|�C���^
	*@param dataSize data�S�̂̃o�C�g�P�ʂ̃T�C�Y
	*Map���ꂽ�̈�Ɉ�����data����������
	*/
	void WriteBuffer(const void* pData, unsigned int amountDatasSize);

	/**
	*@fn
	*@param pData �f�[�^�̃|�C���^
	*@param datasize �f�[�^��̃T�C�Y
	*@param datacount �f�[�^�̑���
	*/
	void WriteBuffer256Alignment(const void* pData, unsigned int datasize, unsigned int datacount);

	
	virtual void SetDescTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle, unsigned int rootParamaterIndex) const;

	void CreateConstantBufferViewDesc();

	void CreateUnorderdAccessViewDesc();

	void CreateShaderResourceViewDesc();

	void CreateRenderTargetViewDesc();

	void CreateDepthStecilViewDesc();

	Microsoft::WRL::ComPtr<ID3D12Resource>& GetBuffer();

	D3D12_RESOURCE_STATES GetCurrentState() const;

	void SetCurrentState(D3D12_RESOURCE_STATES state);

	D3D12_RESOURCE_STATES GetDefaultState() const;

	const std::shared_ptr<Dx12BufferViewDesc>& GetViewDesc() const;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> mBuffer;
	D3D12_RESOURCE_STATES mCurrentState;
	D3D12_RESOURCE_STATES mDeafultState;

	std::shared_ptr<Dx12BufferViewDesc> mViewDescs;

	std::wstring mName;
	unsigned int mElementSize;
	unsigned int mElementCount;

	D3D12_CLEAR_VALUE mClearValue;

#ifdef _DEBUG
	D3D12_HEAP_PROPERTIES mHeapProp;
	D3D12_RESOURCE_DESC mRscDesc;
#endif

	void* mElementBuffer;

	virtual void Map();
};

