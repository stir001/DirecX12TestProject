#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "SwapChainComponent.h"
#include "DescriptorHeapManager.h"
#include "RootSignatureObject.h"
#include "TextureLoader.h"
#include "PiplineStateObject.h"
#include "DepthBufferObject.h"
#include "Dx12Camera.h"
#include "CharToWChar.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <tchar.h>

#include <functional>


#pragma comment(lib,"d3dcompiler.lib")

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


Dx12Ctrl::Dx12Ctrl() :wHeight(720), wWidth(1280), swapchainBufferCount(2),clrcolor{0.5f,0.5f,0.5f,1.0f}
, viewPort{ 0,0,static_cast<float>(wWidth),static_cast<float>(wHeight),0,1.0f},rect{0,0,wWidth,wHeight }
,cmdAllocator(nullptr),cmdList(nullptr),cmdQueue(nullptr),fence(nullptr),factory(nullptr)
,swapchain(nullptr),result(S_OK),fenceValue(0)
,descriptorHeapManager(new DescriptorHeapManager())
{
}

Dx12Ctrl::~Dx12Ctrl()
{
	cmdQueue->Release();
	cmdList->Release();
	cmdAllocator->Release();
	fence->Release();
	factory->Release();
	swapchain->Release();
	delete(swapchain);
	for (int i = 0; i < rootsignature.size(); ++i)
	{
		rootsignature[i]->Release();
		delete(rootsignature[i]);
	}
	dev->Release();
}

Dx12Ctrl* Dx12Ctrl::Instance()
{
	static Dx12Ctrl* dx12Ctrl = nullptr;
	if (dx12Ctrl == nullptr)
	{
		dx12Ctrl = new Dx12Ctrl();
	}

	return dx12Ctrl;
}

ID3D12Device* Dx12Ctrl::GetDev()
{
	return dev;
}

bool Dx12Ctrl::Dx12Init()
{

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	D3D_FEATURE_LEVEL level = {};

	HRESULT result = S_OK;
	for (auto lv : levels)
	{
		result = D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			level = lv;
			break;
		}
	}

	if (result != S_OK)
	{
		dev = nullptr;
		return false;
	}

	InitWindowCreate();

	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));

	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator, nullptr, IID_PPV_ARGS(&cmdList));
	
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	
	result = dev->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	result = CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	CompileShaders();

	depthBuffer = new DepthBufferObject(wWidth, wHeight);
	swapchain = new SwapChainComponent(hWnd);
	descriptorHeapManager = new DescriptorHeapManager();
	//roosignature = new RootSignatureObject();
	texLoader = new TextureLoader();
	camera = new Dx12Camera(wWidth, wHeight);
	CreatePiplineStates();
	


	return true;
}

void Dx12Ctrl::InitWindowCreate()
{
	WNDCLASSEX w = {};
	w.lpfnWndProc = (WNDPROC)WindowProcedure;
	w.lpszClassName = _T("DirecX12APP");
	w.hInstance = NULL;
	w.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&w);

	RECT wrc = { 0,0,wWidth,wHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(w.lpszClassName,
		_T("1601295_真鍋奨一郎"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	hWnd = hwnd;

	ShowWindow(hwnd, SW_SHOW);
}

void  Dx12Ctrl::CreatePiplineStates()
{
	//PiplineStateの作成
	//作成時エラーでvertexShaderとpixelShaderで返り値と引数が異なる場合に発生するものがある
	//このとき引数の型はあっていても内部の変数を使っていない状態でvertexからpixelに渡し、その変数を使った場合も作成時にエラーが出るので注意

	piplinestateObjects = new PiplineStateObject[pso_max];

	//PMD用pso作成
	//vertexShaderに渡す引数のリスト作成
	D3D12_INPUT_ELEMENT_DESC inputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "BONENO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
	};

	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);	//ブレンドするか
	gpsDesc.DepthStencilState.DepthEnable = true;			//???
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = false;		//???
	gpsDesc.InputLayout.NumElements = sizeof(inputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = inputDescs;	//要素へのポインタ(先頭?)
	gpsDesc.pRootSignature = GetRootSignature();				//ルートシグネチャポインタ
	gpsDesc.RasterizerState = rastarizer;	//ラスタライザーの設定
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	gpsDesc.VS = GetShader(si_VS_basic);
	gpsDesc.PS = GetShader(si_PS_notTex);

	piplinestateObjects[pso_notTex].CreatePiplineState(gpsDesc);//3Dモデル テクスチャなしpso作成

	gpsDesc.PS = GetShader(si_PS_exitTex);

	piplinestateObjects[pso_exitTex].CreatePiplineState(gpsDesc);//3Dモデル テクスチャありpso作成

	//primitive用pso作成
	D3D12_INPUT_ELEMENT_DESC primitiveinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	gpsDesc.InputLayout.NumElements = sizeof(primitiveinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = primitiveinputDescs;
	gpsDesc.VS = GetShader(si_VS_primitive);
	gpsDesc.PS = GetShader(si_PS_primitive);

	piplinestateObjects[pso_primitive].CreatePiplineState(gpsDesc);//プリミティブpso作成

	//image用pso作成
	D3D12_INPUT_ELEMENT_DESC imageinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	gpsDesc.InputLayout.NumElements = sizeof(imageinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = imageinputDescs;
	gpsDesc.VS = GetShader(si_VS_image);
	gpsDesc.PS = GetShader(si_PS_image);
	piplinestateObjects[pso_image].CreatePiplineState(gpsDesc);
}

void Dx12Ctrl::CompileShaders()
{
	shaders.resize(si_max);

	//PMDSHADER
	int size = sizeof("Shader.hlsl");
	wchar_t* shaderName = new wchar_t[size];
	size_t convert = ToWChar(&shaderName, size, "Shader.hlsl", size);
	RootSignatureObject* rsObj;


#ifdef _DEBUG
	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
		if (err != nullptr)
		{
		OutputDebugStringA((char*)err->GetBufferPointer());
		}
	};
#else
	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
};
	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

	ID3D10Blob* err = nullptr;
	result = D3DCompileFromFile(shaderName, nullptr, nullptr,
		"BasicVS", "vs_5_0", compileflag, 0, &shaders[si_VS_basic], &err);
	outErr(err);

	result = D3DCompileFromFile(shaderName, nullptr, nullptr,
		"BasicPS", "ps_5_0", compileflag, 0, &shaders[si_PS_notTex], &err);
	outErr(err);

	result = D3DCompileFromFile(shaderName, nullptr, nullptr,
		"ExitTexPS", "ps_5_0", compileflag, 0, &shaders[si_PS_exitTex], &err);
	outErr(err);

	result = D3DCompileFromFile(shaderName, nullptr, nullptr,
		"PrimitiveVS", "vs_5_0", compileflag, 0, &shaders[si_VS_primitive], &err);
	outErr(err);

	result = D3DCompileFromFile(shaderName, nullptr, nullptr,
		"PrimitivePS", "ps_5_0", compileflag, 0, &shaders[si_PS_primitive], &err);
	outErr(err);

	ID3DBlob* rs;
	result = D3DGetBlobPart(shaders[si_VS_basic]->GetBufferPointer(), shaders[si_VS_basic]->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &rs);
	rsObj = new RootSignatureObject(rs);
	rootsignature.push_back(rsObj);

	//IMAGESHADER
	delete(shaderName);
	size = sizeof("ImageShader.hlsl");
	shaderName = new wchar_t[size];
	convert = ToWChar(&shaderName, size, "ImageShader.hlsl", size - 1);

	result = D3DCompileFromFile(shaderName, nullptr, nullptr,
		"ImageVS", "vs_5_0", compileflag, 0, &shaders[si_VS_image], &err);
	outErr(err);
	result = D3DCompileFromFile(shaderName, nullptr, nullptr,
		"ImagePS", "ps_5_0", compileflag, 0, &shaders[si_PS_image], &err);
	outErr(err);

	result = D3DGetBlobPart(shaders[si_VS_image]->GetBufferPointer(), shaders[si_VS_image]->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &rs);
	rsObj = new RootSignatureObject(rs);
	rootsignature.push_back(rsObj);
}

ID3D12CommandAllocator* Dx12Ctrl::GetCmdAllocator()
{
	return cmdAllocator;
}

ID3D12CommandQueue* Dx12Ctrl::GetCmdQueue()
{
	return cmdQueue;
}

ID3D12GraphicsCommandList* Dx12Ctrl::GetCmdList()
{
	return cmdList;
}

ID3D12Fence* Dx12Ctrl::GetFence()
{
	return fence;
}

IDXGIFactory4* Dx12Ctrl::GetFactory()
{
	return factory;
}

UINT64& Dx12Ctrl::GetFenceValue()
{
	return fenceValue;
}

DXGI_SWAP_CHAIN_DESC1 Dx12Ctrl::GetDefaultSwapChainDesc()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Height = wHeight;
	swapChainDesc.Width = wWidth;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = swapchainBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_NONPREROTATED;

	return swapChainDesc;
}

D3D12_STATIC_SAMPLER_DESC Dx12Ctrl::GetDefaulSamplerDesc()
{
	D3D12_STATIC_SAMPLER_DESC rtn;
	rtn.Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	rtn.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	rtn.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	rtn.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	rtn.MaxLOD = D3D12_FLOAT32_MAX;
	rtn.MinLOD = 0.0f;
	rtn.MipLODBias = 0.0f;
	//rtn.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	rtn.ShaderRegister = 0;
	rtn.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rtn.RegisterSpace = 0;
	rtn.MaxAnisotropy = 0;
	rtn.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return rtn;
}

D3D12_DESCRIPTOR_HEAP_DESC  Dx12Ctrl::MakeDescriptorHeapDesc(DescriptorHeapDescType type, int numDescriptor)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtn;
	switch (type)
	{
	case dhdt_dsv:
		rtn.NumDescriptors = numDescriptor;
		rtn.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		rtn.NodeMask = 0;
		break;
	case dhdt_cbv:
		rtn.NumDescriptors = numDescriptor;
		rtn.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		rtn.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		rtn.NodeMask = 0;
		break;
	case dhdt_rtv:
		rtn.NumDescriptors = numDescriptor;
		rtn.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtn.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtn.NodeMask = 0;
		break;
	case dhdt_srv:
		rtn.NumDescriptors = numDescriptor;
		rtn.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		rtn.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		rtn.NodeMask = 0;
		break;
	default:
		rtn.NumDescriptors = numDescriptor;
		break;
	}

	return rtn;
}

DescriptorHeapManager* Dx12Ctrl::GetDescriptorHeapManager()
{
	return descriptorHeapManager;
}

ID3D12RootSignature* Dx12Ctrl::GetRootSignature(int index)
{
	return rootsignature[index]->GetRootSignature();
}

IDXGISwapChain3* Dx12Ctrl::GetSwapChain()
{
	return swapchain->GetSwapChain();
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Ctrl::GetCurrentRTVHeap()
{
	return swapchain->GetCurrentRTVHeap();
}

ID3D12Resource* Dx12Ctrl::GetCurrentRenderTarget()
{
	return swapchain->GetCurrentRenderTarget();
}

TextureObj* Dx12Ctrl::LoadTexture(std::wstring filepath, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle)
{
	return texLoader->LoadTexture(filepath,cpuHandle,gpuhandle);
}

ID3D12PipelineState* Dx12Ctrl::GetPiplineState(PSOIndex index)
{
	return piplinestateObjects[index].GetPiplineState();
}

D3D12_SHADER_BYTECODE Dx12Ctrl::GetShader(ShaderIndex index)
{
	return CD3DX12_SHADER_BYTECODE(shaders[index]);
}

void Dx12Ctrl::SwapChainPresent(UINT SyncInterval, UINT flags)
{
	swapchain->SwapChainPresent(SyncInterval, flags);
}

void Dx12Ctrl::CmdQueueSignal()
{
	cmdQueue->Signal(fence, ++fenceValue);
	UINT64 value = fence->GetCompletedValue();
	UINT64 u64max = UINT64_MAX;
	while (value != fenceValue)
	{
		value = fence->GetCompletedValue();
		if(value == UINT64_MAX)GetDeviceRemoveReason();
	}
}

HRESULT Dx12Ctrl::GetDeviceRemoveReason()
{
	return result = dev->GetDeviceRemovedReason();
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

void Dx12Ctrl::InitMainCmdList()
{
	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator, GetPiplineState(pso_notTex));
	D3D12_CPU_DESCRIPTOR_HANDLE rtvhandle = GetCurrentRTVHeap();
	cmdList->OMSetRenderTargets(1, &rtvhandle, false, &depthBuffer->GetCPUAdress());
	cmdList->ClearDepthStencilView(depthBuffer->GetCPUAdress() , D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, &rect);
	cmdList->ClearRenderTargetView(rtvhandle, clrcolor, 0, &rect);
	cmdList->SetGraphicsRootSignature(GetRootSignature());

	cmdList->RSSetViewports(1, &viewPort);
	cmdList->RSSetScissorRects(1, &rect);
}

ID3D12DescriptorHeap* Dx12Ctrl::CreateTextureDescHeap(std::string modelPath, int texturecount)
{
	return texLoader->CreateTexDescHeap(modelPath, texturecount);
}

void Dx12Ctrl::SetCameraBuffer()
{
	camera->SetBuffer();
}

void Dx12Ctrl::ExcuteAndPresent()
{
	GetCmdList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentRenderTarget(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT)); //読み込み中に書き込みをされないようにする
	GetCmdList()->Close();

	{
		ID3D12CommandList* loacalcmdlist[] = { GetCmdList() };
		GetCmdQueue()->ExecuteCommandLists(_countof(loacalcmdlist), loacalcmdlist);
		CmdQueueSignal();//通常
	}

	SwapChainPresent(1, 0);
}

//テスト用関数
Dx12Camera * Dx12Ctrl::GetCamera()
{
	return camera;
}