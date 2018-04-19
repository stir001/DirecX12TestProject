#include "Dx12Ctrl.h"
#include "Geometry.h"
#include "PMDLoader.h"
#include "PrimitiveManager.h"
#include "PMDController.h"
#include "VMDLoader.h"
#include "Dx12Camera.h"
#include "DxInput.h"
#include "DirectionalLight.h"
#include "ImageLoader.h"
#include "ImageObject.h"

#include <algorithm>
#include <Windows.h>
#include <tchar.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <map>

using namespace DirectX;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const TCHAR* APP_CLASS_NAME = _T("DirectX12Test");
const int SCREEN_BUFFER_COUNT = 2;

//LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR, int cmdShow)
{
	///Direct3D12の初期化

	HRESULT result = S_OK;

	Dx12Ctrl::Instance()->Dx12Init();
	Dx12Ctrl* d12 = Dx12Ctrl::Instance();

	//PMXLoader loader;
	//TgaLoader tgaLoader;
	//std::string tgapath = "Tda式ミク/k_face_MikuAp.tga";
	//tgaLoader.LoadImageData(tgapath);

	//std::string path = "Tda式ミク/Tda式ミク.pmx";
	//loader.Load(path);

	PMDLoader pmdloader;
	std::string modelpath = "初音ミク.pmd";
	//std::string modelpath = "博麗霊夢/reimu_F01.pmd";
	PMDController* model = pmdloader.Load(modelpath);

	MSG msg = {};

	std::string vmdPath = "vmd/charge.vmd";

	VMDLoader vmdloader;
	VMDMotion* motion = vmdloader.LoadMotion(vmdPath);
	model->SetMotion(motion);
	model->PlayMotion(true);
	DirectionalLight* dirLight = new DirectionalLight(DirectX::XMFLOAT3(-20, 0, -20), DirectX::XMFLOAT3(1.5, -0.5, 1));

	ImageLoader imgLoader;
	std::string imgpath = "texturesample.bmp";
	ImageObject* imgObject = imgLoader.LoadImageData(imgpath);
#pragma region  シャドウマップ関係 未実装
	//D3D12_RANGE cbvRange = { 0,0 };

	////シャドウマップ用バッファ確保
	//ID3D12DescriptorHeap* shadowDescHeap = nullptr;
	//ID3D12Resource* shadowBuffer = nullptr;

	//D3D12_DESCRIPTOR_HEAP_DESC shadowmapHeapDesc = {};
	//shadowmapHeapDesc.NumDescriptors = 1;
	//shadowmapHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	//d12->result = d12->GetDev()->CreateDescriptorHeap(&shadowmapHeapDesc, IID_PPV_ARGS(&shadowDescHeap));
	//D12RESULTCHECK

	//unsigned int screensize = max(WINDOW_WIDTH, WINDOW_HEIGHT);

	//screensize = Roundup2Multiplier(screensize);

	//D3D12_RESOURCE_DESC shadowmapResourceDesc = {};
	//shadowmapResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//shadowmapResourceDesc.Width = screensize;
	//shadowmapResourceDesc.Height = screensize;
	//shadowmapResourceDesc.Format = DXGI_FORMAT_R32_TYPELESS;//D32_FLOATだと後で使いにくいため
	//shadowmapResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//shadowmapResourceDesc.DepthOrArraySize = 1;
	//shadowmapResourceDesc.MipLevels = 1;
	//shadowmapResourceDesc.SampleDesc.Count = 1;
	//shadowmapResourceDesc.SampleDesc.Quality = 0;
	//shadowmapResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//D3D12_CLEAR_VALUE clearValue = {};
	//clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	//clearValue.DepthStencil.Depth = 1.0f;
	//clearValue.DepthStencil.Stencil = 0;

	////D3D12_HEAP_PROPERTIES shadowHeapProp = {};
	////shadowHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	////shadowHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	////shadowHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	////shadowHeapProp.CreationNodeMask = 0;
	////shadowHeapProp.VisibleNodeMask = 0;

	//d12->result = d12->GetDev()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
	//	D3D12_HEAP_FLAG_NONE,
	//	&shadowmapResourceDesc,
	//	D3D12_RESOURCE_STATE_DEPTH_WRITE,
	//	&clearValue,
	//	IID_PPV_ARGS(&shadowBuffer));
	//D12RESULTCHECK

	////深度バッファ用descHeap
	//D3D12_DESCRIPTOR_HEAP_DESC shadowmapDSV_DHD = {};
	//shadowmapDSV_DHD.NumDescriptors = 1;
	//shadowmapDSV_DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	//ID3D12DescriptorHeap* shadowDsvDH = nullptr;
	//d12->result = d12->GetDev()->CreateDescriptorHeap(&shadowmapDSV_DHD, IID_PPV_ARGS(&shadowDsvDH));
	//D12RESULTCHECK

	////シェーダーリソース用descHeap
	//D3D12_DESCRIPTOR_HEAP_DESC shadowmapSRV_DHD = {};
	//shadowmapSRV_DHD.NumDescriptors = 1;
	//shadowmapSRV_DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//shadowmapSRV_DHD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//ID3D12DescriptorHeap* shadowSrvDH = nullptr;
	//d12->result = d12->GetDev()->CreateDescriptorHeap(&shadowmapSRV_DHD, IID_PPV_ARGS(&shadowSrvDH));
	//D12RESULTCHECK

	////デプスと関連付ける
	//D3D12_DEPTH_STENCIL_VIEW_DESC shadowDSVDesc = {};
	//shadowDSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//shadowDSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//d12->GetDev()->CreateDepthStencilView(shadowBuffer, &shadowDSVDesc, shadowDsvDH->GetCPUDescriptorHandleForHeapStart());

	////シェーダーリソースと関連付け
	//D3D12_SHADER_RESOURCE_VIEW_DESC s_srvDesc = {};
	//s_srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//s_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//s_srvDesc.Texture2D.MipLevels = 1;
	//s_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//d12->GetDev()->CreateShaderResourceView(shadowBuffer, &s_srvDesc, shadowSrvDH->GetCPUDescriptorHandleForHeapStart());

	//D3D12_VIEWPORT s_viewPort = {};
	//s_viewPort.Width = static_cast<float>(screensize);
	//s_viewPort.Height = static_cast<float>(screensize);
	//D3D12_RECT  s_rect = {};
	//s_rect.right = screensize;
	//s_rect.bottom = screensize;

	//float angle = 0.f;
	//unsigned int frame = 0;
#pragma endregion

	PrimitiveManager priMgr;
	priMgr.CreatePlane(XMFLOAT3(0, 0, 0), 60, 60, XMFLOAT3(0, 1, 0));

	priMgr.CreatePlane(XMFLOAT3(0, 30, 30), 60, 60, XMFLOAT3(0, 0, -1));

	priMgr.CreatePlane(XMFLOAT3(30, 30, 0), 60, 60, XMFLOAT3(-1, 0, 0));
	
	int offsetX = 0;
	XMFLOAT3 offsetPos = { (float)offsetX,0,0 };

	D3D12_VIEWPORT port;
	port.TopLeftX = 0;
	port.TopLeftY = 0;
	port.MaxDepth = 1.0f;
	port.MinDepth = 0.0f;
	port.Height = WINDOW_HEIGHT / 2;
	port.Width = WINDOW_WIDTH / 2;

	D3D12_VIEWPORT mainPort = 
		{ static_cast<float>(WINDOW_WIDTH) / 2
		, static_cast<float>(WINDOW_HEIGHT) / 2
		, static_cast<float>(WINDOW_WIDTH) / 2.0f
		, static_cast<float>(WINDOW_HEIGHT) / 2.0f
		, 0
		, 1.0f };

	DxInput input;

	MousePos pos;
	XMFLOAT3 cPos = { 0,20,-30 };

	Dx12Camera* camera = d12->GetCamera();

	//メインループ
	while (true) {
		CallStartPerGameLoop();

		input.UpdateKeyState();

		pos = input.GetMousePos();

		if (input.GetKeyState()[vki_LSHIFT] & ksc_DOWN)
		{
			camera->MoveUp(-0.1f);
		}

		if (input.GetKeyState()[vki_SPACE] & ksc_DOWN)
		{
			camera->MoveUp(0.1f);
		}

		if (input.GetKeyState()[vki_W] & ksc_DOWN)
		{
			camera->MoveFront(0.1f);
		}

		if (input.GetKeyState()[vki_S] & ksc_DOWN)
		{
			camera->MoveFront(-0.1f);
		}

		if (input.GetKeyState()[vki_A] & ksc_DOWN)
		{
			camera->MoveSide(0.1f);
		}

		if (input.GetKeyState()[vki_D] & ksc_DOWN)
		{
			camera->MoveSide(-0.1f);
		}

		if (input.GetKeyState()[vki_LEFT] & ksc_DOWN)
		{
			camera->TurnRightLeft(2.0f);
		}

		if (input.GetKeyState()[vki_RIGHT] & ksc_DOWN)
		{
			camera->TurnRightLeft(-2.0f);
		}

		if (input.GetKeyState()[vki_UP] & ksc_DOWN)
		{
			camera->TurnUpDown(2.0f);
		}

		if (input.GetKeyState()[vki_DOWN] & ksc_DOWN)
		{
			camera->TurnUpDown(-2.0f);
		}

		//camera->SetPos(cPos);
		//offsetPos.x = static_cast<float>(abs(100 - (int)(++offsetX) % 200));
		//world = DirectX::XMMatrixRotationY(angle);
		//angle += 0.01f;

		//シャドウマップ用処理
		//d12->GetCmdAllocator()->Reset();
		//d12->GetCmdList()->Reset(d12->GetCmdAllocator(), pipleinestate);
		//d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature());

		//d12->GetCmdList()->ClearDepthStencilView(shadowDsvDH->GetCPUDescriptorHandleForHeapStart(),
		//	D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		//d12->GetCmdList()->OMSetRenderTargets(0, nullptr, false, &shadowDsvDH->GetCPUDescriptorHandleForHeapStart());

		//d12->GetCmdList()->RSSetViewports(1, &s_viewPort);
		//d12->GetCmdList()->RSSetScissorRects(1, &s_rect);
		//cBuffer->diffuse = XMFLOAT3(0, 0, 0);
		//cBuffer->alpha = 1.0f;
		//cBuffer->specularity = 0.0f;
		//cBuffer->specular = XMFLOAT3(0, 0, 0);
		//cBuffer->ambient = XMFLOAT3(0, 0, 0);
		//cBuffer->existTexture = false;
		//{
		//	XMFLOAT3 tmpPos = { offsetPos.x / 10,0,0 };
		//	cBuffer->offset = tmpPos;
		//}

		//d12->GetCmdList()->SetDescriptorHeaps(1, &cbvDescHeap);
		//d12->GetCmdList()->SetGraphicsRootDescriptorTable(rpt_cbv, cbvDescHeap->GetGPUDescriptorHandleForHeapStart());
		//model->SetIndexBuffer();
		//model->SetVertexBuffer();

		//d12->GetCmdList()->SetDescriptorHeaps(1, &boneCBVDescHeap);
		//d12->GetCmdList()->SetGraphicsRootDescriptorTable(rpt_bonematrix, boneCBVDescHeap->GetGPUDescriptorHandleForHeapStart());

		//d12->GetCmdList()->DrawIndexedInstanced(model->indices.size(), 1, 0, 0, 0);
		//
		//d12->GetCmdList()->ResourceBarrier(1
		//	, &CD3DX12_RESOURCE_BARRIER::Transition(shadowBuffer,
		//		D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		//		D3D12_RESOURCE_STATE_PRESENT));
		//d12->GetCmdList()->Close();

		//{
		//	ID3D12CommandList* cmdlist[] = { d12->GetCmdList() };
		//	d12->GetCmdQueue()->ExecuteCommandLists(_countof(cmdlist), cmdlist);
		//	d12->CmdQueueSignal();//シャドウ
		//}

		//通常描画
		//d12->GetCmdList()->SetDescriptorHeaps(1, &shadowSrvDH);
		//d12->GetCmdList()->SetGraphicsRootDescriptorTable(rpt_shadow, shadowSrvDH->GetGPUDescriptorHandleForHeapStart());
		//d12->SetCameraBuffer();
		//d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pso_notTex));

		dirLight->SetLight();
		model->Draw();

		priMgr.Draw();

		CallEndPerGameLoop();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

	}
	d12->GetDev()->Release();
	UnregisterClass(APP_CLASS_NAME, hInst);
}

//LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//	if (msg == WM_DESTROY) {
//		PostQuitMessage(0);
//		return 0;
//	}
//	return DefWindowProc(hwnd, msg, wparam, lparam);
//}