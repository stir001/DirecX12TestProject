#include "ImageLoader.h"
#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "CharToWChar.h"
#include "TextureObject.h"
#include "ImageController.h"
#include "RootSignatureObject.h"
#include "PipelineStateObject.h"
#include "ShaderCompiler.h"
#include "TextureLoader.h"
#include "RenderingPathManager.h"
#include "Image3DController.h"

#include "d3dx12.h"

const std::string IMAGE_SHADER_PATH = "DirectX12/ImageShader.hlsl";
const std::string IMAGE_VERTEXSHADER_NAME = "ImageVS";
const std::string IMAGE_PIXCELSHADER_NAME = "ImagePS";
const std::string IMAGE_GEOMETRYSHADER_NAME = "";
const std::string IMAGE_HULLSHADER_NAME = "";
const std::string IMAGE_DOMAINSHADER_NAME = "";

std::unique_ptr<ImageLoader> ImageLoader::mInstance = nullptr;

ImageLoader::ImageLoader()
{
	DX12CTRL_INSTANCE;
	CreateRootsignature(d12->GetDev());
	CreatePipelineState(d12->GetDev());
	mCmdList = RenderingPathManager::Instance()->GetRenderingPathCommandList(0);
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
		imgCtrl.reset(new ImageController(itr->second, Dx12Ctrl::Instance()->GetDev(), mCmdList, mPipelinestate, mRootsignature));
		return imgCtrl;
	}
	
	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance()->LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj(new ImageObject(tObj->GetWidth(), tObj->GetHeight(), tObj));
	mImages[path] = imgObj;
	imgCtrl.reset(new ImageController(imgObj, Dx12Ctrl::Instance()->GetDev(), mCmdList,mPipelinestate,mRootsignature));

	return imgCtrl;
}

std::shared_ptr<Image3DController> ImageLoader::LoadImage3D(const std::string& path)
{
	std::shared_ptr<Image3DController> imgCtrl;
	auto itr = mImages.find(path);
	if (itr != mImages.end())
	{
		imgCtrl.reset(new Image3DController(itr->second, Dx12Ctrl::Instance()->GetDev(), mCmdList, m3DPipelinestate, m3DRootsignature));
		return imgCtrl;
	}

	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance()->LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj(new ImageObject(tObj->GetWidth(), tObj->GetHeight(), tObj));
	mImages[path] = imgObj;
	imgCtrl.reset(new Image3DController(imgObj, Dx12Ctrl::Instance()->GetDev(), mCmdList, m3DPipelinestate, m3DRootsignature));

	return imgCtrl;
}

void ImageLoader::Release(const std::string& releaseImagePath)
{
	TextureLoader::Instance()->Release(releaseImagePath);
	mImages.erase(releaseImagePath);
}

void ImageLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	//image�ppso�쐬
	D3D12_INPUT_ELEMENT_DESC imageinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
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
	gpsDesc.BlendState = blendDesc;	//�u�����h���邩
	gpsDesc.DepthStencilState.DepthEnable = false;			//�f�v�X���g����
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = false;		//???
	gpsDesc.InputLayout.NumElements = sizeof(imageinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = imageinputDescs;	//�v�f�ւ̃|�C���^(�擪?)
	gpsDesc.pRootSignature = mRootsignature->GetRootSignature().Get();				//���[�g�V�O�l�`���|�C���^
	gpsDesc.RasterizerState = rastarizer;	//���X�^���C�U�[�̐ݒ�
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(mShader.vertexShader.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(mShader.pixcelShader.Get());

	
	mPipelinestate.reset(new PipelineStateObject(gpsDesc, dev));

	D3D12_INPUT_ELEMENT_DESC image3DinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	gpsDesc.InputLayout.pInputElementDescs = image3DinputDescs;
	gpsDesc.InputLayout.NumElements = sizeof(image3DinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);

	gpsDesc.DepthStencilState.DepthEnable = true;
	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(m3DShader.vertexShader.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(m3DShader.pixcelShader.Get());
	gpsDesc.pRootSignature = m3DRootsignature->GetRootSignature().Get();

	m3DPipelinestate.reset(new PipelineStateObject(gpsDesc, dev));
}

void ImageLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mShader = ShaderCompiler::GetInstance()->CompileShader(
		IMAGE_SHADER_PATH,
		IMAGE_VERTEXSHADER_NAME,
		IMAGE_PIXCELSHADER_NAME,
		IMAGE_GEOMETRYSHADER_NAME,
		IMAGE_HULLSHADER_NAME,
		IMAGE_DOMAINSHADER_NAME,
		true);
	mRootsignature.reset(new RootSignatureObject(mShader.rootSignature.Get(),dev));
	mRootsignature->GetRootSignature()->SetName(L"ImageRootSignature");

	m3DShader = ShaderCompiler::GetInstance()->CompileShader(
		"DirectX12/Image3DShader.hlsl",
		"Image3DVS",
		"Image3DPS",
		"",
		"",
		"",
		true);
	m3DRootsignature.reset(new RootSignatureObject(m3DShader.rootSignature.Get(), dev));
	m3DRootsignature->GetRootSignature()->SetName(L"Image3DRootSignature");
}