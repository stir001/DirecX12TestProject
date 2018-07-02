#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <memory>
#include <vector>
#include <string>
#include <wrl.h>
#include <DirectXMath.h>

#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG



#define DX12CTRL_INSTANCE Dx12Ctrl* d12 = Dx12Ctrl::Instance();



class SwapChainObject;
class DescriptorHeapManager;
class RootSignatureObject;
class TextureLoader;
class PipelineStateObject;
class TextureObject;
class DepthBufferObject;
class Dx12Camera;
class Dx12DescriptorHeapObject;

namespace DirectX
{
 struct XMFLOAT2;
};

enum PSOIndex
{
	pso_notTex,//textureなしのポリゴン描画用(3Dモデル)
	pso_exitTex,//textureありのポリゴン描画用(3Dモデル)
	pso_primitive,//プリミティブ描画
	pso_max
};

enum RootSignatureIndex
{
	rsi_pmd,
	rsi_prm,
	rsi_max
};

enum ShaderIndex
{
	si_VS_basic,
	si_PS_notTex,
	si_PS_exitTex,
	si_VS_primitive,
	si_PS_primitive,
	si_max
};

class Dx12Ctrl
{
public:
	~Dx12Ctrl();

	static Dx12Ctrl* Instance()
	{
		if (inst == nullptr)
		{
			inst = new Dx12Ctrl();
		}

		return inst;
	}

	static void Destroy()
	{
		delete inst;
	}

	HRESULT result;

	Microsoft::WRL::ComPtr<ID3D12Device> GetDev();
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCmdAllocator();
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCmdQueue();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	Microsoft::WRL::ComPtr<IDXGIFactory4> GetFactory();
	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature(int index = 0);

	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(PSOIndex index);
	D3D12_SHADER_BYTECODE GetShader(ShaderIndex index);
	std::shared_ptr<DepthBufferObject> GetDepthBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthCpuHandle() const;

	void CmdQueueSignal();
	UINT64 GetFenceValue() const;
	bool Dx12Init(HINSTANCE winHInstance);
	DirectX::XMFLOAT2 GetWindowSize() const;
	void SetWindowSize(int inw, int inh);
	void SetWindowName(std::string& inWindowName);

	const D3D12_VIEWPORT& GetViewPort() const;
	const D3D12_RECT& GetRect() const;

	const HWND& GetWndHandle() const;

	HRESULT GetDeviceRemoveReason();
	HRESULT CheckResult();
	HRESULT CheckResult(HRESULT r);

	void Release();

	//テスト用関数
	std::shared_ptr<Dx12Camera> GetCamera() const;

	template<typename T>
	LONG CheckComPtrRefCount(Microsoft::WRL::ComPtr<T>& comptr)
	{
		LONG count = comptr.Get()->AddRef() - 1;
		comptr.Get()->Release();
		return count;
	}

	void SetInstanceHandle(HINSTANCE& hInstance) {
		mWinHInstance = hInstance;
	};

private:
	Dx12Ctrl();
	Dx12Ctrl(const Dx12Ctrl&);
	Dx12Ctrl(const Dx12Ctrl&&);
	Dx12Ctrl& operator=(const Dx12Ctrl&);

	static Dx12Ctrl* inst;

	HINSTANCE mWinHInstance;
	int mWndHeight;
	int mWndWidth;
	std::string mWindowName;
	Microsoft::WRL::ComPtr<ID3D12Device> mDev;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	Microsoft::WRL::ComPtr<IDXGIFactory4> mFactory;
	std::vector<std::shared_ptr<RootSignatureObject>> mRootsignature;
	std::vector<std::shared_ptr<PipelineStateObject>> mPipelinestateObjects;
	std::shared_ptr<DepthBufferObject> mDepthBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDepthDescHeap;
	std::shared_ptr<Dx12Camera> mCamera;
	std::vector<Microsoft::WRL::ComPtr<ID3DBlob>> mShaders;
	UINT64 mFenceValue;
	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mRect;
	HWND mhWnd;
	float mClrcolor[4];

	void CreatePipelineStates();
	void CompileShaders();
	void InitWindowCreate();
	void InitFirstPath();
};