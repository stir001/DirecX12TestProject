#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "SwapChainObject.h"
#include "RootSignatureObject.h"
#include "TextureLoader.h"
#include "PipelineStateObject.h"
#include "DepthBufferObject.h"
#include "Dx12Camera.h"
#include "CharToWChar.h"
#include "HlslInclude.h"
#include "CharToWChar.h"
#include "Util.h"
#include "RenderingPathManager.h"
#include "Dx12CommandList.h"
#include "Dx12DescriptorHeapObject.h"
//#include "FbxLoader.h"
#include "ImageLoader.h"
#include "ShaderCompiler.h"
#include "Primitive2DManager.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <tchar.h>
#include <DirectXMath.h>
#include <string>
#include <functional>
#include <Windows.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#pragma comment(lib,"d3dcompiler.lib")

Dx12Ctrl* Dx12Ctrl::inst = nullptr;

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

HRESULT Dx12Ctrl::ReportLiveObject()
{
#ifdef _DEBUG
	return mDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
#else
	return S_OK;
#endif
}

Dx12Ctrl::Dx12Ctrl() :mWndHeight(720), mWndWidth(1280),mClrcolor{0.5f,0.5f,0.5f,1.0f}
, mViewPort{ 0,0,static_cast<float>(mWndWidth),static_cast<float>(mWndHeight),0,1.0f},mRect{0,0,mWndWidth,mWndHeight }
,mCmdAllocator(nullptr),mCmdList(nullptr),mCmdQueue(nullptr),mFactory(nullptr)
,result(S_OK),mFenceValue(0)
,mWindowName("DirectX12")
{
	setlocale(LC_ALL, "japanese");
}

Dx12Ctrl::~Dx12Ctrl()
{
	Release();
	//ReportLiveObject();
#ifdef _DEBUG
	mDebugDevice.Reset();
#endif // _DEBUG
	mDev.Reset();
}

Microsoft::WRL::ComPtr<ID3D12Device> Dx12Ctrl::GetDev()
{	
	return mDev;
}

bool Dx12Ctrl::Dx12Init( HINSTANCE winHInstance)
{
#ifdef _DEBUG
	{
		ID3D12Debug* debug;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
		debug->EnableDebugLayer();
		debug->Release();
	}
#endif
	//
	mWinHInstance = winHInstance;
	//

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	D3D_FEATURE_LEVEL level = {};

	HRESULT result = S_OK;

	result = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));

	Microsoft::WRL::ComPtr<IDXGIAdapter1>	hardwareAdapter;
	Microsoft::WRL::ComPtr<IDXGIAdapter1>	adapter;
	hardwareAdapter = nullptr;

	std::wstring searchStr = L"NVIDIA";

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != mFactory->EnumAdapters1(i, &adapter); i++) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		std::wstring description = desc.Description;
		size_t size = description.find(searchStr);
		if (size >= description.size())
		{
			continue;
		}

		for (auto i : levels) {
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), i, _uuidof(ID3D12Device), nullptr))) {
				level = i;
				hardwareAdapter = adapter;
				break;
			}
		}
	}

	if (hardwareAdapter)
	{
		result = D3D12CreateDevice(hardwareAdapter.Get(), level, IID_PPV_ARGS(&mDev));
	}
	else
	{
		for (auto i : levels) {
			if (SUCCEEDED(D3D12CreateDevice(nullptr, i, IID_PPV_ARGS(&mDev)))) {
				level = i;
				break;
			}
		}
	}
#ifdef _DEBUG
	//mDev->QueryInterface(mDebugDevice.GetAddressOf());
#endif // _DEBUG
	adapter.Detach();

	if (result != S_OK)
	{
		mDev = nullptr;
		return false;
	}




	mDev->SetName(L"ID3D12Device");

	InitWindowCreate();

	mDepthBuffer.reset(new DepthBufferObject("MasterDepthBuffer", mDev, mWndWidth, mWndHeight));
	std::vector<std::shared_ptr<Dx12BufferObject>> t_buffer;
	t_buffer.push_back(mDepthBuffer);
	mDepthDescHeap.reset(new Dx12DescriptorHeapObject("DepthDescriptorHeap",mDev, t_buffer, D3D12_DESCRIPTOR_HEAP_TYPE_DSV));
	t_buffer.clear();

	result = mDev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCmdAllocator));
	result = mDev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&mCmdList));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = mDev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue));
	
	result = mDev->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));

	CompileShaders();

	mCamera.reset(new Dx12Camera(mWndWidth, mWndHeight));
	//CreatePipelineStates();

	//RendringManagerクラスの初期化処理
	InitFirstPath();

	return true;

}

void Dx12Ctrl::InitFirstPath()
{
	RenderingPathManager::Instance()->Init(mDev, mFactory, mhWnd);

	InitFunctionObject_t initfunc = [&](CmdListsArg_t cmdList, RTResourcesArg_t resource, RTDescHeapArg_t descHeap) {
		cmdList[0]->ClearDepthStencilView(Dx12Ctrl::Instance()->GetDepthCpuHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, &mRect);
		cmdList[0]->OMSetRenderTargets(1, &descHeap->GetCPUDescriptorHandleForHeapStart(), false, &Dx12Ctrl::Instance()->GetDepthCpuHandle());
		cmdList[0]->ClearRenderTargetView(descHeap->GetCPUDescriptorHandleForHeapStart(), mClrcolor, 0, &mRect);
		cmdList[0]->RSSetViewports(1, &mViewPort);
		cmdList[0]->RSSetScissorRects(1, &mRect);
	};

	RenderingPathManager::Instance()->SetInitFunction(0, initfunc);

	LastFunctionObject_t lastFunc = [](CmdListsArg_t cmdList, RTResourcesArg_t resrouce) {

	};

	RenderingPathManager::Instance()->SetLastFunction(0, lastFunc);
}

void Dx12Ctrl::InitWindowCreate()
{
	WNDCLASSEX w = {};
	w.lpfnWndProc = (WNDPROC)WindowProcedure;
	w.lpszClassName = _T("Holojection");
	w.hInstance = mWinHInstance;
	w.hIcon = LoadIcon(w.hInstance, _T("HOLOJECTION"));
	w.cbSize = sizeof(WNDCLASSEX);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon;
	RegisterClassEx(&w);

	RECT wrc = { 0,0,mWndWidth,mWndHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	const char* name = mWindowName.data();
	std::string strName;
	strName. reserve(mWindowName.size());
	for (auto& s : mWindowName)
	{
		strName.push_back(s);
	}
	strName.push_back('\0');
	size_t size = strName.size();
	wchar_t* buff = nullptr;
	ToWChar(&buff, size, strName.data(), size);

	HWND hwnd = CreateWindow(w.lpszClassName,
		buff,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	mhWnd = hwnd;

	delete buff;

	ShowWindow(hwnd, SW_SHOW);
}

void  Dx12Ctrl::CreatePipelineStates()
{
	//PipelineStateの作成
	//作成時エラーでvertexShaderとpixelShaderで返り値と引数が異なる場合に発生するものがある
	//このとき引数の型はあっていても内部の変数を使っていない状態でvertexからpixelに渡し、その変数を使った場合も作成時にエラーが出るので注意

	//std::shared_ptr<PipelineStateObject> pso;

	////PMD用pso作成
	////vertexShaderに渡す引数のリスト作成
	//D3D12_INPUT_ELEMENT_DESC inputDescs[] = {
	//	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//	{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//	{ "BONENO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//	{ "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
	//};

	//CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//rastarizer.CullMode = D3D12_CULL_MODE_NONE;

	//D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	//gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);	//ブレンドするか
	//gpsDesc.DepthStencilState.DepthEnable = true;			//デプスを使うか
	//gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	//gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//gpsDesc.DepthStencilState.StencilEnable = false;		//???
	//gpsDesc.InputLayout.NumElements = sizeof(inputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	//gpsDesc.InputLayout.pInputElementDescs = inputDescs;	//要素へのポインタ(先頭?)
	//gpsDesc.pRootSignature = GetRootSignature().Get();				//ルートシグネチャポインタ
	//gpsDesc.RasterizerState = rastarizer;	//ラスタライザーの設定
	//gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//
	//gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//gpsDesc.SampleDesc.Count = 1;
	//gpsDesc.NumRenderTargets = 1;
	//gpsDesc.SampleMask = 0xffffff;
	//gpsDesc.NodeMask = 0;

	//gpsDesc.VS = GetShader(si_VS_basic);
	//gpsDesc.PS = GetShader(si_PS_notTex);
	//gpsDesc.DS;
	//gpsDesc.GS;
	//gpsDesc.HS;
	//
	////3Dモデル テクスチャなしpso作成
	//pso.reset(new PipelineStateObject(gpsDesc, mDev));
	//mPipelinestateObjects.push_back(pso);


	//gpsDesc.PS = GetShader(si_PS_exitTex);

	//pso.reset(new PipelineStateObject(gpsDesc, mDev));
	//mPipelinestateObjects.push_back(pso);//3Dモデル テクスチャありpso作成


	////primitive用pso作成
	//D3D12_INPUT_ELEMENT_DESC primitiveinputDescs[] = {
	//	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//	{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//	{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	//};

	//gpsDesc.InputLayout.NumElements = sizeof(primitiveinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	//gpsDesc.InputLayout.pInputElementDescs = primitiveinputDescs;
	//gpsDesc.VS = GetShader(si_VS_primitive);
	//gpsDesc.PS = GetShader(si_PS_primitive);
	//gpsDesc.pRootSignature = GetRootSignature(rsi_prm).Get();

	//pso.reset(new PipelineStateObject(gpsDesc, mDev));
	//mPipelinestateObjects.push_back(pso);//プリミティブpso作成

}

void Dx12Ctrl::CompileShaders()
{
//	mShaders.resize(si_max);
//
//	//PMDSHADER
//	int size = sizeof("DirectX12/Shader.hlsl");
//	wchar_t* shaderName;
//	size_t convert = ToWChar(&shaderName, size, "DirectX12/Shader.hlsl", size);
//	std::shared_ptr<RootSignatureObject> rsObj;
//
//	HlslInclude hlslinculde;
//
//	hlslinculde.SetRelativePath(GetRelativePath("DirectX12/Shader.hlsl"));
//
//#ifdef _DEBUG
//	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
//	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
//		if (err != nullptr)
//		{
//		OutputDebugStringA((char*)err->GetBufferPointer());
//		err->Release();
//		}
//	};
//#else
//	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
//};
//	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS;
//#endif
//
//	ID3D10Blob* err = nullptr;
//	result = D3DCompileFromFile(shaderName, nullptr, &hlslinculde,
//		"BasicVS", "vs_5_0", compileflag, 0, &mShaders[si_VS_basic], &err);
//	outErr(err);
//
//	result = D3DCompileFromFile(shaderName, nullptr, &hlslinculde,
//		"BasicPS", "ps_5_0", compileflag, 0, &mShaders[si_PS_notTex], &err);
//	outErr(err);
//
//	result = D3DCompileFromFile(shaderName, nullptr, &hlslinculde,
//		"ExitTexPS", "ps_5_0", compileflag, 0, &mShaders[si_PS_exitTex], &err);
//	outErr(err);
//
//	ID3DBlob* rs;
//	result = D3DGetBlobPart(mShaders[si_VS_basic]->GetBufferPointer(), mShaders[si_VS_basic]->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &rs);
//	rsObj.reset(new RootSignatureObject(rs,mDev));
//	mRootsignature.push_back(rsObj);
//
//	rs->Release();
//
//	delete(shaderName);
//	size = sizeof("DirectX12/Primitive3D.hlsl");
//	convert = ToWChar(&shaderName, size, "DirectX12/Primitive3D.hlsl", size);
//
//	result = D3DCompileFromFile(shaderName, nullptr, &hlslinculde,
//		"PrimitiveVS", "vs_5_0", compileflag, 0, &mShaders[si_VS_primitive], &err);
//	outErr(err);
//
//	result = D3DCompileFromFile(shaderName, nullptr, &hlslinculde,
//		"PrimitivePS", "ps_5_0", compileflag, 0, &mShaders[si_PS_primitive], &err);
//	outErr(err);
//
//	result = D3DGetBlobPart(mShaders[si_VS_primitive]->GetBufferPointer(), mShaders[si_VS_primitive]->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &rs);
//	rsObj.reset(new RootSignatureObject(rs,mDev));
//	mRootsignature.push_back(rsObj);
//
//	rs->Release();
//
//	delete shaderName;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> Dx12Ctrl::GetCmdAllocator()
{
	return mCmdAllocator;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> Dx12Ctrl::GetCmdQueue()
{
	return mCmdQueue;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Dx12Ctrl::GetCmdList()
{
	return mCmdList;
}

Microsoft::WRL::ComPtr<IDXGIFactory4> Dx12Ctrl::GetFactory()
{
	return mFactory;
}

UINT64 Dx12Ctrl::GetFenceValue() const
{
	return mFenceValue;
}

//Microsoft::WRL::ComPtr<ID3D12RootSignature> Dx12Ctrl::GetRootSignature(int index)
//{
//	return mRootsignature[index]->GetRootSignature();
//}


//Microsoft::WRL::ComPtr<ID3D12PipelineState> Dx12Ctrl::GetPipelineState(PSOIndex index)
//{
//	return mPipelinestateObjects[index]->GetPipelineState();
//}

D3D12_SHADER_BYTECODE Dx12Ctrl::GetShader(ShaderIndex index)
{
	return CD3DX12_SHADER_BYTECODE(mShaders[index].Get());
}


void Dx12Ctrl::CmdQueueSignal()
{
	mCmdQueue->Signal(mFence.Get(), ++mFenceValue);
	UINT64 value = mFence->GetCompletedValue();
	UINT64 u64max = UINT64_MAX;
	while (value != mFenceValue)
	{
		value = mFence->GetCompletedValue();
		if (value == UINT64_MAX)
		{
			GetDeviceRemoveReason();
		}
	}
}

HRESULT Dx12Ctrl::GetDeviceRemoveReason()
{
	result = mDev->GetDeviceRemovedReason();
	if (FAILED(result))
	{
		MessageBox(mhWnd, L"Device removed", L"ERROR", MB_OK);
	}
	return result;
}

HRESULT Dx12Ctrl::CheckResult()
{
	if (FAILED(result))
	{
		return GetDeviceRemoveReason();
	}
	return result;
}
HRESULT Dx12Ctrl::CheckResult(HRESULT r)
{
	return GetDeviceRemoveReason();
}

DirectX::XMFLOAT2 Dx12Ctrl::GetWindowSize() const
{
	DirectX::XMFLOAT2 size = { static_cast<float>(mWndWidth), static_cast<float>(mWndHeight) };
	return size;
}

void Dx12Ctrl::Release()
{
	mCmdAllocator.Reset();
	mCmdList.Reset();
	mCmdQueue.Reset();
	mFactory.Reset();
	mFence.Reset();
	//mRootsignature.clear();
	//mPipelinestateObjects.clear();
	mShaders.clear();
	mDepthBuffer.reset();
	mDepthDescHeap.reset();
	TextureLoader::Destroy();
	RenderingPathManager::Destroy();
	//FbxLoader::Destroy();
	ImageLoader::Destroy();
	ShaderCompiler::Destroy();
	mCamera.reset();
	Primitive2DManager::Destory();
}

void Dx12Ctrl::SetWindowSize(int inw, int inh)
{
	mWndWidth = inw;
	mWndHeight = inh;
	mViewPort.Width = static_cast<float>(inw);
	mViewPort.Height = static_cast<float>(inh);
	mRect.right = inw;
	mRect.bottom = inh;
}

void Dx12Ctrl::SetWindowName(std::string& inWindowName)
{
	mWindowName = inWindowName;
}

const HWND& Dx12Ctrl::GetWndHandle() const
{
	return mhWnd;
}

std::shared_ptr<DepthBufferObject> Dx12Ctrl::GetDepthBuffer() const
{
	return mDepthBuffer;
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Ctrl::GetDepthCpuHandle() const
{
	return mDepthDescHeap->GetCPUHeapHandleStart();
}

//テスト用関数
std::shared_ptr<Dx12Camera> Dx12Ctrl::GetCamera() const
{
	return mCamera;
}

const D3D12_VIEWPORT& Dx12Ctrl::GetViewPort() const
{
	return mViewPort;
}

const D3D12_RECT& Dx12Ctrl::GetRect() const
{
	return mRect;
}