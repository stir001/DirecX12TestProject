#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <memory>
#include <vector>
#include <string>


#define DX12CTRL_INSTANCE Dx12Ctrl* d12 = Dx12Ctrl::Instance();

#ifdef _DEBUG
#define D12RESULTCHECK Dx12Ctrl::Instance()->CheckResult();
#else
#define D12RESULTCHECK 

#endif

class SwapChainComponent;
class DescriptorHeapManager;
class RootSignatureObject;
class TextureLoader;
class PiplineStateObject;
class TextureObj;
class DepthBufferObject;
class Dx12Camera;

namespace DirectX
{
 struct XMFLOAT2;
};

enum DescriptorHeapDescType
{
	dhdt_dsv,//depth stencil view desc
	dhdt_cbv,//constant buffer view desc
	dhdt_rtv,//render target view desc
	dhdt_srv,//shader resource view desc
};

enum PSOIndex
{
	pso_notTex,//textureなしのポリゴン描画用(3Dモデル)
	pso_exitTex,//textureありのポリゴン描画用(3Dモデル)
	pso_primitive,//プリミティブ描画
	pso_image,
	pso_max
};

enum RootSignatureIndex
{
	rsi_pmd,
	rsi_prm,
	rsi_image,
	rsi_max
};

enum ShaderIndex
{
	si_VS_basic,
	si_PS_notTex,
	si_PS_exitTex,
	si_VS_primitive,
	si_PS_primitive,
	si_VS_image,
	si_PS_image,
	si_max
};

class Dx12Ctrl
{
private:
	int wHeight;
	int wWidth;
	int swapchainBufferCount;
	ID3D12Device* dev;
	ID3D12CommandAllocator* cmdAllocator;
	ID3D12CommandQueue* cmdQueue;
	ID3D12GraphicsCommandList* cmdList;
	ID3D12Fence* fence;
	IDXGIFactory4* factory;
	SwapChainComponent* swapchain;
	DescriptorHeapManager* descriptorHeapManager;
	std::vector<RootSignatureObject*> rootsignature;
	PiplineStateObject* piplinestateObjects;
	DepthBufferObject* depthBuffer;
	Dx12Camera* camera;
	std::vector<ID3DBlob*> shaders;
	TextureLoader* texLoader;
	UINT64 fenceValue;
	D3D12_VIEWPORT viewPort;
	D3D12_RECT rect;
	HWND hWnd;
	float clrcolor[4];
	void CreatePiplineStates();
	void CompileShaders();
	void InitWindowCreate();

	Dx12Ctrl();
	Dx12Ctrl(const Dx12Ctrl&);
public:
	static Dx12Ctrl* Instance();
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC1 GetDefaultSwapChainDesc();
	D3D12_STATIC_SAMPLER_DESC GetDefaulSamplerDesc();
	D3D12_DESCRIPTOR_HEAP_DESC MakeDescriptorHeapDesc(DescriptorHeapDescType type,int numDescriptor = 1);
	ID3D12Device* GetDev();
	ID3D12CommandAllocator* GetCmdAllocator();
	ID3D12CommandQueue* GetCmdQueue();
	ID3D12GraphicsCommandList* GetCmdList();
	ID3D12Fence* GetFence();
	IDXGIFactory4* GetFactory();
	DescriptorHeapManager* GetDescriptorHeapManager();
	ID3D12RootSignature* GetRootSignature(int index = 0);
	IDXGISwapChain3* GetSwapChain();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeap();
	ID3D12Resource* GetCurrentRenderTarget();
	TextureObj* LoadTexture(std::wstring path, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle);
	ID3D12PipelineState* GetPiplineState(PSOIndex index);
	D3D12_SHADER_BYTECODE GetShader(ShaderIndex index);
	ID3D12DescriptorHeap* CreateTextureDescHeap(std::string modelPath, int texturecount);
	void SetCameraBuffer();
	void SwapChainPresent(UINT SyncInterval, UINT flags);
	void CmdQueueSignal();
	UINT64& GetFenceValue();
	bool Dx12Init();
	void InitMainCmdList();
	void ExcuteAndPresent();
	DirectX::XMFLOAT2 GetWindowSize();

	HRESULT GetDeviceRemoveReason();
	HRESULT CheckResult();
	HRESULT CheckResult(HRESULT r);

	//テスト用関数
	Dx12Camera* GetCamera();

	~Dx12Ctrl();
};

