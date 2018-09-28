#include "stdafx.h"
#include "ImageLoader.h"
#include "DrawObject/Image/ImageObject.h"
#include "Master/Dx12Ctrl.h"
#include "Util/CharToWChar.h"
#include "Texture/TextureObject.h"
#include "DrawObject/Image/ImageController.h"
#include "Rootsignature/RootSignatureObject.h"
#include "PipelineState/PipelineStateObject.h"
#include "Shader/ShaderCompiler.h"
#include "Texture/TextureLoader.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include "DrawObject/Image/Image3DController.h"

#include "d3dx12.h"

const std::string IMAGE_SHADER_PATH = SHADERDIR_PATH + "ImageShader.hlsl";
const std::string IMAGE_VERTEXSHADER_NAME = "ImageVS";
const std::string IMAGE_PIXCELSHADER_NAME = "ImagePS";
const std::string IMAGE_GEOMETRYSHADER_NAME = "";
const std::string IMAGE_HULLSHADER_NAME = "";
const std::string IMAGE_DOMAINSHADER_NAME = "";

ImageLoader* ImageLoader::mInstance = nullptr;

ImageLoader::ImageLoader()
{
	DX12CTRL_INSTANCE;
	CreateRootsignature(d12.GetDev());
	CreatePipelineState(d12.GetDev());
	mCmdList = RenderingPathManager::Instance().GetRenderingPathCommandList(0);
}

ImageLoader::~ImageLoader()
{
	mImages.clear();
	mRootsignature.reset();
	mPipelinestate.reset();
	m3DPipelinestate.reset();
	m3DPipelinestate.reset();
}

std::shared_ptr<ImageController> ImageLoader::LoadImageData(const std::string& path)
{
	std::shared_ptr<ImageController> imgCtrl;
	auto itr = mImages.find(path);
	if (itr != mImages.end())
	{
		imgCtrl = std::make_shared<ImageController>(itr->second, Dx12Ctrl::Instance().GetDev(), mCmdList, mPipelinestate, mRootsignature);
		return imgCtrl;
	}
	
	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance().LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj = ImageObject::Create(tObj->GetWidth(), tObj->GetHeight(), tObj);
	mImages[path] = imgObj;
	imgCtrl = std::make_shared<ImageController>(imgObj, Dx12Ctrl::Instance().GetDev(), mCmdList,mPipelinestate,mRootsignature);

	return imgCtrl;
}

std::shared_ptr<Image3DController> ImageLoader::LoadImage3D(const std::string& path)
{
	std::shared_ptr<Image3DController> imgCtrl;
	auto itr = mImages.find(path);
	if (itr != mImages.end())
	{
		imgCtrl = std::make_shared<Image3DController>(itr->second, Dx12Ctrl::Instance().GetDev(), mCmdList, m3DPipelinestate, m3DRootsignature);
		return imgCtrl;
	}

	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance().LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj = ImageObject::Create(tObj->GetWidth(), tObj->GetHeight(), tObj);
	mImages[path] = imgObj;
	imgCtrl = std::make_shared<Image3DController>(imgObj, Dx12Ctrl::Instance().GetDev(), mCmdList, m3DPipelinestate, m3DRootsignature);

	return imgCtrl;
}

void ImageLoader::Release(const std::string& releaseImagePath)
{
	TextureLoader::Instance().Release(releaseImagePath);
	mImages.erase(releaseImagePath);
}

void ImageLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	//image用pso作成
	D3D12_INPUT_ELEMENT_DESC imageinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "GAMMA",0,DXGI_FORMAT_R32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;

	D3D12_BLEND_DESC blendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;


	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = blendDesc;	//ブレンドするか
	gpsDesc.DepthStencilState.DepthEnable = false;			//デプスを使うか
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = false;		//???
	gpsDesc.InputLayout.NumElements = sizeof(imageinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = imageinputDescs;	//要素へのポインタ(先頭?)
	gpsDesc.pRootSignature = mRootsignature->GetRootSignature().Get();				//ルートシグネチャポインタ
	gpsDesc.RasterizerState = rastarizer;	//ラスタライザーの設定
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(mShader.vertexShader.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(mShader.pixelShader.Get());

	
	mPipelinestate = std::make_shared <PipelineStateObject>("Image2D",gpsDesc, dev);

	D3D12_INPUT_ELEMENT_DESC image3DinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{"GAMMA",0,DXGI_FORMAT_R32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	gpsDesc.InputLayout.pInputElementDescs = image3DinputDescs;
	gpsDesc.InputLayout.NumElements = sizeof(image3DinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);

	gpsDesc.DepthStencilState.DepthEnable = true;
	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(m3DShader.vertexShader.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(m3DShader.pixelShader.Get());
	gpsDesc.pRootSignature = m3DRootsignature->GetRootSignature().Get();

	m3DPipelinestate = std::make_shared<PipelineStateObject>("Image3D",gpsDesc, dev);
}

void ImageLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::Instance().CompileShader(
		IMAGE_SHADER_PATH,
		IMAGE_VERTEXSHADER_NAME,
		IMAGE_PIXCELSHADER_NAME,
		IMAGE_GEOMETRYSHADER_NAME,
		IMAGE_HULLSHADER_NAME,
		IMAGE_DOMAINSHADER_NAME,
		true);
	mRootsignature = std::make_shared<RootSignatureObject>("Image2D",mShader.rootSignature.Get(),dev);
	mRootsignature->GetRootSignature()->SetName(L"ImageRootSignature");

	m3DShader = ShaderCompiler::Instance().CompileShader(
		SHADERDIR_PATH + "Image3DShader.hlsl",
		"Image3DVS",
		"Image3DPS",
		"",
		"",
		"",
		true);
	m3DRootsignature = std::make_shared<RootSignatureObject>("Image3D",m3DShader.rootSignature.Get(), dev);
	m3DRootsignature->GetRootSignature()->SetName(L"Image3DRootSignature");
}
