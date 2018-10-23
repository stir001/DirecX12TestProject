#include "stdafx.h"
#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxModelController.h"
#include "FbxModelDataCoverter.h"
#include "FbxMotionConverter.h"
#include "FbxMotionData.h"
#include "Shader/ShaderCompiler.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Master/Dx12Ctrl.h"
#include "PipelineState/PipelineStateObject.h"
#include "Util/XMFloatOperators.h"
#include "Light/DirectionalLight.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include "d3dx12.h"


#include <fbxsdk.h>
#include <memory>
#include <exception>
#include <cassert>
#include <algorithm>
#include <Windows.h>
#include <cassert>

FbxLoader* FbxLoader::mInstance = nullptr;

const std::string FBX_SHADER_PATH = SHADERDIR_PATH + "FbxShader.hlsl";
const std::string FBX_VERTEXSHADER_NAME = "FbxVS";
const std::string FBX_PIXCELSHADER_NAME = "FbxPS";
const std::string FBX_GEOMETRYSHADER_NAME = "";
const std::string FBX_HULLSHADER_NAME = "";
const std::string FBX_DOMAINSHADER_NAME = "";

FbxLoader::FbxLoader():mModelConverter(std::make_shared<FbxModelDataConverter>()),mMotionConverter(std::make_shared<FbxMotionConverter>()),mLight(std::make_shared<DirectionalLight>(1.0f,-1.0f,1.0f))
{
	mShader = ShaderCompiler::Instance().CompileShader(
		FBX_SHADER_PATH,
		FBX_VERTEXSHADER_NAME,
		FBX_PIXCELSHADER_NAME,
		FBX_GEOMETRYSHADER_NAME,
		FBX_HULLSHADER_NAME,
		FBX_DOMAINSHADER_NAME,
		true);

	DX12CTRL_INSTANCE;
	CreateRootsignature(d12.GetDev());
	CreatePipelineState(d12.GetDev());
	mCmdList = RenderingPathManager::Instance().GetRenderingPathCommandList(0);
}

void FbxLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_BACK;

	D3D12_INPUT_ELEMENT_DESC fbxinputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);	//ブレンドするか
	gpsDesc.DepthStencilState.DepthEnable = true;			//デプスを使うか
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DepthStencilState.StencilEnable = false;		//???
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.InputLayout.NumElements = sizeof(fbxinputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = fbxinputDescs;
	gpsDesc.pRootSignature = mRootsignature->GetRootSignature().Get();//ルートシグネチャポインタ
	gpsDesc.RasterizerState = rastarizer;	//ラスタライザーの設定
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(mShader.vertexShader.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(mShader.pixelShader.Get());
	gpsDesc.DS;
	gpsDesc.GS;
	gpsDesc.HS;

	mPipelinestate = std::make_shared<PipelineStateObject>("FbxPipelineState",gpsDesc,dev);
}

void FbxLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mRootsignature = std::make_shared<RootSignatureObject>("FbxRootSignature",mShader.rootSignature.Get(), dev);
	mRootsignature->GetRootSignature()->SetName(L"FbxRootSignature");
}

void FbxLoader::SetPipelineState(std::shared_ptr<PipelineStateObject>& Pipelinestate)
{
	mPipelinestate = Pipelinestate;
}

void FbxLoader::SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
}

FbxLoader::~FbxLoader()
{
	mModelConverter = nullptr;
	mMotionConverter = nullptr;
	mModelDatas.clear();
	mModelDatas.clear();
}

bool FbxLoader::LoaderInitializie(std::string fbxPath)
{
	//SDKマネージャの作成

	mManager = fbxsdk::FbxManager::Create();
	//IOセッティングの設定

	fbxsdk::FbxIOSettings* ioSettings = fbxsdk::FbxIOSettings::Create(mManager, IOSROOT);
	mManager->SetIOSettings(ioSettings);

	fbxsdk::FbxString lPath = fbxsdk::FbxGetApplicationDirectory();
	mManager->LoadPluginsDirectory(lPath.Buffer());

	//シーンの作成
	mScene = fbxsdk::FbxScene::Create(mManager, "Scene");

	//インポーターの作成
	mImporter = fbxsdk::FbxImporter::Create(mManager, "importer");
	int fileFormat = -1;
	if (!mManager->GetIOPluginRegistry()->DetectReaderFileFormat(fbxPath.data(), fileFormat))
	{
		fileFormat = mManager->GetIOPluginRegistry()->FindReaderIDByDescription("Fbx binary (*.fbx)");
	}

	//インポーター初期化
	if (!mImporter->Initialize(fbxPath.c_str(), fileFormat)) {
		MessageBox(nullptr, L"Load Error <Imprter is null>", L"FBXModelLoader", MB_OK);
		return false;
	}

	//ロードしたファイルをシーンにインポート
	mImporter->Import(mScene);

	fbxsdk::FbxAxisSystem sceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();

	fbxsdk::FbxAxisSystem directXAxisSys(fbxsdk::FbxAxisSystem::eDirectX);

	fbxsdk::FbxAxisSystem maxAxisSys(fbxsdk::FbxAxisSystem::Max);
	fbxsdk::FbxAxisSystem mayaAxisSysY(fbxsdk::FbxAxisSystem::MayaYUp);
	fbxsdk::FbxAxisSystem mayaAxisSysZ(fbxsdk::FbxAxisSystem::MayaZUp);

	fbxsdk::FbxAxisSystem system = maxAxisSys;
	if (sceneAxisSystem != system)
	{
		system.ConvertScene(mScene);
	}

	return true;
}

std::shared_ptr<FbxModelController> FbxLoader::LoadMesh(const std::string& modelPath)
{
	std::shared_ptr<FbxModelController> rtn(nullptr);
	auto itr = mModelDatas.find(modelPath);
	if (itr != mModelDatas.end())
	{
		rtn = std::make_shared<FbxModelController>(itr->second, Dx12Ctrl::Instance().GetDev(), mCmdList, mPipelinestate, mRootsignature);
		return rtn;
	}

	if (!LoaderInitializie(modelPath))
	{
		return nullptr;
	}

	//const int poseCount = mScene->GetPoseCount();
	//if (poseCount > 1)
	//{
	//	return nullptr;
	//}
	//else
	//{
	//	mPose = mScene->GetPose(0);
	//}

	//ルートノードのゲット
	fbxsdk::FbxNode* rootNode = mScene->GetRootNode();

	if (rootNode) {
		FbxAMatrix dummy;
		dummy.SetIdentity();
		NodeTree rNode;
		rNode.nodeName = rootNode->GetName();
		fbxsdk::FbxTime t = 0;
		//rNode.globalPosition = GetGlobalPosition(rootNode,t,mPose,&dummy);
		//rNode.globalOffsetPosition = rNode.globalPosition * GetGeometry(rootNode);
		mNodeTree = rNode;
		StackSearchNode(rootNode, FbxNodeAttribute::EType::eMesh, mNodeTree, [&](fbxsdk::FbxNode* node) {
			mNodeDatas.push_back(node);
			mMeshDatas.push_back(node->GetMesh());
		});
	}
	else {
		MessageBox(nullptr, L"Load Error <rootNode is null>", L"FBXModelLoader", MB_OK);
		return nullptr;
	}
	if (mMeshDatas.size() == 0)
	{
		MessageBox(nullptr, L"メッシュがないよ", L"FBXModelLoader", MB_OK);
		return rtn;
	}
	
	std::vector<std::shared_ptr<Fbx::FbxModelData>> models(mMeshDatas.size());

	for (int i = 0; i < static_cast<int>(mMeshDatas.size()); i++)
	{
		FbxVector4 t0 = mNodeDatas[i]->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 r0 = mNodeDatas[i]->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 s0 = mNodeDatas[i]->GetGeometricScaling(FbxNode::eSourcePivot);
		mGeometryOffset = FbxAMatrix(t0, r0, s0);
		models[i] = MainLoad(mMeshDatas[i], modelPath);
		models[i]->modelPath = modelPath;
	}
	

	//変更必要
	
	std::shared_ptr<Fbx::FbxModel> model(mModelConverter->ConvertToFbxModel(ConnectMeshes(models)));
	mModelDatas[modelPath] = model;
	rtn = std::make_shared<FbxModelController>(model, Dx12Ctrl::Instance().GetDev(), mCmdList, mPipelinestate, mRootsignature);
	rtn->SetLight(mLight);

	DestroyScence(mScene);
	mManager->Destroy();
	ClearTmpInfo();

	return rtn;
}

std::shared_ptr<FbxMotionData> FbxLoader::LoadAnimation(const std::string& animationPath)
{

	if (!LoaderInitializie(animationPath))
	{
		return nullptr;
	}

	FbxGeometryConverter geometryConverter(mManager);
	geometryConverter.Triangulate(mScene, true);

	const int poseCount = mScene->GetPoseCount();
	if (poseCount != 1)
	{
		return nullptr;
	}
	else
	{
		mPose = mScene->GetPose(0);
	}

	fbxsdk::FbxNode* rootNode = mScene->GetRootNode();
	if (rootNode) {
		FbxAMatrix dummy;
		dummy.SetIdentity();
		NodeTree rNode;
		rNode.nodeName = rootNode->GetName();
		fbxsdk::FbxTime t = 0;
		//rNode.globalPosition = GetGlobalPosition(rootNode, t, mPose, &dummy);
		//rNode.globalOffsetPosition = rNode.globalPosition * GetGeometry(rootNode);
		mNodeTree = rNode;
		StackSearchNode(rootNode, FbxNodeAttribute::EType::eMesh, mNodeTree, [&](fbxsdk::FbxNode* node) {
			mNodeDatas.push_back(node);
			mMeshDatas.push_back(node->GetMesh());
		});
	}
	else {
		MessageBox(nullptr, L"Load Error <rootNode is null>", L"FBXModelLoader", MB_OK);
		return nullptr;
	}

	fbxsdk::FbxArray<fbxsdk::FbxString*> mAnimStacknameArray;

	mScene->FillAnimStackNameArray(mAnimStacknameArray);

	fbxsdk::FbxArray<fbxsdk::FbxNode*> cameraArray;

	cameraArray.Clear();

	StackNode(mScene->GetRootNode(), fbxsdk::FbxNodeAttribute::eCamera, cameraArray);

	FbxVector4 t0 = mNodeDatas[0]->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 r0 = mNodeDatas[0]->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 s0 = mNodeDatas[0]->GetGeometricScaling(FbxNode::eSourcePivot);
	mGeometryOffset = FbxAMatrix(t0, r0, s0);

	LoadAnimationMain(mScene, 0);

	fbxsdk::FbxTakeInfo* pTakeInfo = mImporter->GetTakeInfo(0);
	fbxsdk::FbxLongLong start = pTakeInfo->mLocalTimeSpan.GetStart().Get();
	fbxsdk::FbxLongLong stop = pTakeInfo->mLocalTimeSpan.GetStop().Get();

	fbxsdk::FbxLongLong max = (stop - start) / fbxsdk::FbxTime::GetOneFrameValue(fbxsdk::FbxTime::eFrames60);

	std::shared_ptr<FbxMotionData> motion(mMotionConverter->ConvertMotionData(mSkeletonMatrix, max, animationPath));

	mAnimationDatas[animationPath] = motion;

	DestroyScence(mScene);
	mManager->Destroy();
	ClearTmpInfo();

	return motion;
}

void FbxLoader::ReleaseModel(const std::string& modelPath)
{
	if (mModelDatas.find(modelPath) != mModelDatas.end())
	{
		mModelDatas.erase(modelPath);
	}
}

void FbxLoader::ReleaseNonRefModel()
{
	std::vector<std::string> keyValue;
	for (auto& model : mModelDatas)
	{
		if (model.second.use_count() == 1)
		{
			keyValue.push_back(model.first);
		}
	}

	for (auto& key : keyValue)
	{
		mModelDatas.erase(key);
	}
}

void FbxLoader::ReleaseAllModel()
{
	mModelDatas.clear();
}


void FbxLoader::ReleaseAnimation(const std::string& animationPath)
{
	if (mAnimationDatas.find(animationPath) != mAnimationDatas.end())
	{
		mAnimationDatas.erase(animationPath);
	}
}

void FbxLoader::ReleaseNonRefAnimation()
{
	std::vector<std::string> keyValue;
	for (auto& anim : mAnimationDatas)
	{
		if (anim.second.use_count() == 1)
		{
			keyValue.push_back(anim.first);
		}
	}

	for (auto& key : keyValue)
	{
		mAnimationDatas.erase(key);
	}
}

void FbxLoader::ReleaseAllAnimation()
{
	mAnimationDatas.clear();
}

//テスト用ローカル関数
void StoreFbxMatrixToXMMatrix(const fbxsdk::FbxAMatrix& fmat, DirectX::XMMATRIX& xmmat)
{
	//DirectXと同じ状態ならこう
	xmmat.r[0].m128_f32[0] = static_cast<float>(fmat.Get(0, 0)); xmmat.r[0].m128_f32[1] = static_cast<float>(fmat.Get(0, 1)); xmmat.r[0].m128_f32[2] = static_cast<float>(fmat.Get(0, 2)); xmmat.r[0].m128_f32[3] = static_cast<float>(fmat.Get(0, 3));
	xmmat.r[1].m128_f32[0] = static_cast<float>(fmat.Get(1, 0)); xmmat.r[1].m128_f32[1] = static_cast<float>(fmat.Get(1, 1)); xmmat.r[1].m128_f32[2] = static_cast<float>(fmat.Get(1, 2)); xmmat.r[1].m128_f32[3] = static_cast<float>(fmat.Get(1, 3));
	xmmat.r[2].m128_f32[0] = static_cast<float>(fmat.Get(2, 0)); xmmat.r[2].m128_f32[1] = static_cast<float>(fmat.Get(2, 1)); xmmat.r[2].m128_f32[2] = static_cast<float>(fmat.Get(2, 2)); xmmat.r[2].m128_f32[3] = static_cast<float>(fmat.Get(2, 3));
	xmmat.r[3].m128_f32[0] = static_cast<float>(fmat.Get(3, 0)); xmmat.r[3].m128_f32[1] = static_cast<float>(fmat.Get(3, 1)); xmmat.r[3].m128_f32[2] = static_cast<float>(fmat.Get(3, 2)); xmmat.r[3].m128_f32[3] = static_cast<float>(fmat.Get(3, 3));

	//転置されているならこう
	/*xmmat.r[0].m128_f32[0] = static_cast<float>(fmat.Get(0, 0)); xmmat.r[0].m128_f32[1] = static_cast<float>(fmat.Get(1, 0)); xmmat.r[0].m128_f32[2] = static_cast<float>(fmat.Get(2, 0)); xmmat.r[0].m128_f32[3] = static_cast<float>(fmat.Get(3, 0));
	xmmat.r[1].m128_f32[0] = static_cast<float>(fmat.Get(0, 1)); xmmat.r[1].m128_f32[1] = static_cast<float>(fmat.Get(1, 1)); xmmat.r[1].m128_f32[2] = static_cast<float>(fmat.Get(2, 1)); xmmat.r[1].m128_f32[3] = static_cast<float>(fmat.Get(3, 1));
	xmmat.r[2].m128_f32[0] = static_cast<float>(fmat.Get(0, 2)); xmmat.r[2].m128_f32[1] = static_cast<float>(fmat.Get(1, 2)); xmmat.r[2].m128_f32[2] = static_cast<float>(fmat.Get(2, 2)); xmmat.r[2].m128_f32[3] = static_cast<float>(fmat.Get(3, 2));
	xmmat.r[3].m128_f32[0] = static_cast<float>(fmat.Get(0, 3)); xmmat.r[3].m128_f32[1] = static_cast<float>(fmat.Get(1, 3)); xmmat.r[3].m128_f32[2] = static_cast<float>(fmat.Get(2, 3)); xmmat.r[3].m128_f32[3] = static_cast<float>(fmat.Get(3, 3));*/
}

void FbxLoader::StackNode(fbxsdk::FbxNode* pNode, FbxNodeAttribute::EType type, fbxsdk::FbxArray<fbxsdk::FbxNode*>& nodeArray)
{
	if (pNode)
	{
		if (pNode->GetNodeAttribute())
		{
			if (pNode->GetNodeAttribute()->GetAttributeType() == type)
			{
				nodeArray.Add(pNode);
			}
			const int count = pNode->GetChildCount();
			for (int i = 0; i < count; ++i)
			{
				StackNode(pNode->GetChild(i), type, nodeArray);
			}
		}
	}
}

void FbxLoader::LoadVertexPosition(fbxsdk::FbxMesh* mesh)
{
	//START vertex position load
	int ctlPointcount = mesh->GetControlPointsCount();
	mTmpVertices.clear();
	mTmpVertices.shrink_to_fit();
	mTmpVertices.resize(ctlPointcount);

	fbxsdk::FbxVector4* vertexBuffer = mesh->GetControlPoints();
	for (int i = 0; i < ctlPointcount; i++)
	{
		mTmpVertices[i].pos.x = static_cast<float>(vertexBuffer[i][0]);
		mTmpVertices[i].pos.y = static_cast<float>(vertexBuffer[i][1]);
		mTmpVertices[i].pos.z = static_cast<float>(vertexBuffer[i][2]);
	}
	//END vertex position load 
}

void FbxLoader::LoadVertexNormal(fbxsdk::FbxMesh* mesh)
{
	int* polygonvertices = mesh->GetPolygonVertices();
	int polygonverticesCount = mesh->GetPolygonVertexCount();

	std::vector<int> normalArrayIndices;
	std::vector<int> vertexIndices;
	normalArrayIndices.resize(polygonverticesCount);
	vertexIndices.resize(polygonverticesCount);
	fbxsdk::FbxGeometryElementNormal* normalElement = nullptr;
	fbxsdk::FbxGeometryElement::EReferenceMode referenceMode;
	fbxsdk::FbxGeometryElement::EMappingMode mappingMode;
	fbxsdk::FbxVector4 t_normal;
	Fbx::TmpNormalUV nluv;

	//START vertex normal load
	int normallayercount = mesh->GetElementNormalCount();
	for (int layer = 0; layer < normallayercount; layer++)
	{

		normalElement = mesh->GetElementNormal(layer);
		referenceMode = normalElement->GetReferenceMode();
		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>* normalArray = &normalElement->GetDirectArray();
		mappingMode = normalElement->GetMappingMode();


		fbxsdk::FbxLayerElementArrayTemplate<int>* normalArrayIndicesArray = &normalElement->GetIndexArray();//normalArrayに対応するIndexを格納している配列


		int eIndexToDirectCount = 0;


		switch (mappingMode)
		{
		case fbxsdk::FbxLayerElement::eNone://マッピングが未定　対応できないふざけんな
			break;
		case fbxsdk::FbxLayerElement::eByControlPoint://コントロールポイントと1対1対応
			switch (referenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect://eByControllPointの場合はイ頂点の順番に定義すればいいはず
				for (int i = 0; i < static_cast<int>(mTmpVertices.size()); ++i)
				{
					mTmpVertices[i].normalandUV.resize(1);

					t_normal = normalArray->GetAt(i);
					mTmpVertices[i].normalandUV[0].normal.x = static_cast<float>(t_normal[0]);
					mTmpVertices[i].normalandUV[0].normal.y = static_cast<float>(t_normal[1]);
					mTmpVertices[i].normalandUV[0].normal.z = static_cast<float>(t_normal[2]);
					mTmpVertices[i].normalandUV[0].vertexNo = i;
					}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect://おそらくこれは来ない？

				eIndexToDirectCount = normalArrayIndicesArray->GetCount();
				for (int i = 0; i < eIndexToDirectCount; ++i)
				{
					t_normal = normalArray->GetAt(normalArrayIndicesArray->GetAt(i));
					nluv.normal.x = static_cast<float>(t_normal[0]);
					nluv.normal.y = static_cast<float>(t_normal[1]);
					nluv.normal.z = static_cast<float>(t_normal[2]);
					nluv.uv.x = 0;
					nluv.uv.y = 0;
					nluv.vertexNo = i;
					mTmpVertices[i].normalandUV.push_back(nluv);
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex://ポリゴンごとのバーテックス情報と1対t1対応　インデックス配列分用意されている？
			switch (referenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect://eByPolygonVertexの場合はPolygonVertexの順番でバーッてクスを定義すればいいはず
				for (int i = 0; i < polygonverticesCount; ++i)
				{

					t_normal = normalArray->GetAt(i);
					nluv.normal.x = static_cast<float>(t_normal[0]);
					nluv.normal.y = static_cast<float>(t_normal[1]);
					nluv.normal.z = static_cast<float>(t_normal[2]);
					nluv.vertexNo = polygonvertices[i];
					mTmpVertices[polygonvertices[i]].normalandUV.push_back(nluv);
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < polygonverticesCount; ++i)
				{

					normalArrayIndices[i] = normalArrayIndicesArray->GetAt(i);
					vertexIndices[i] = polygonvertices[i];

					t_normal = normalArray->GetAt(normalArrayIndicesArray->GetAt(i));
					nluv.normal.x = static_cast<float>(t_normal[0]);
					nluv.normal.y = static_cast<float>(t_normal[1]);
					nluv.normal.z = static_cast<float>(t_normal[2]);
					nluv.uv.x = 0;
					nluv.uv.y = 0;
					nluv.vertexNo = polygonvertices[i];
					mTmpVertices[polygonvertices[i]].normalandUV.push_back(nluv);
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygon://ポリゴンごとに共通
			break;
		case fbxsdk::FbxLayerElement::eByEdge://よくわからん
			break;
		case fbxsdk::FbxLayerElement::eAllSame://全部一緒
			break;
		default:
			break;
		}
	}
	//END vertex normal load
	if (normalElement != nullptr)
	{
		normalElement = nullptr;
	}
}

void FbxLoader::LoadVertexUV(fbxsdk::FbxMesh* mesh)
{
	int* polygonvertices = mesh->GetPolygonVertices();
	int polygonverticesCount = mesh->GetPolygonVertexCount();
	std::vector<int> vertexRefCount(mTmpVertices.size());
	std::fill(vertexRefCount.begin(), vertexRefCount.end(), 0);

	std::vector<int> uvlArrayIndices;
	std::vector<int> vertexIndices;
	uvlArrayIndices.resize(polygonverticesCount);
	vertexIndices.resize(polygonverticesCount);

	//START vertex uv load

	fbxsdk::FbxStringList t_UVSetNameList;
	mesh->GetUVSetNames(t_UVSetNameList);
	int uvSetCount = t_UVSetNameList.GetCount();
	for (int uvSetIndex = 0; uvSetIndex < uvSetCount; ++uvSetIndex)
	{
		const char* t_uvSetName = t_UVSetNameList.GetStringAt(uvSetIndex);

		const fbxsdk::FbxGeometryElementUV* t_uvElement = mesh->GetElementUV(t_uvSetName);

		if (t_uvElement == nullptr) continue;


		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector2>* uvArray = &t_uvElement->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int>* uvArrayIndicesArray = &t_uvElement->GetIndexArray();
		int uvArrayIndicesCount = uvArrayIndicesArray->GetCount();

		fbxsdk::FbxGeometryElement::EReferenceMode uvReferenceMode = t_uvElement->GetReferenceMode();
		fbxsdk::FbxGeometryElement::EMappingMode uvMappingMode = t_uvElement->GetMappingMode();

		DirectX::XMFLOAT2 t_uv;

		switch (uvMappingMode)
		{
		case fbxsdk::FbxLayerElement::eNone:
			break;
		case fbxsdk::FbxLayerElement::eByControlPoint:
			switch (uvReferenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect:
				for (int i = 0; i < static_cast<int>(mTmpVertices.size()); ++i)
				{

					mTmpVertices[i].normalandUV[0].uv.x = static_cast<float>(uvArray->GetAt(i)[0]);
					mTmpVertices[i].normalandUV[0].uv.y = 1.0f - static_cast<float>(uvArray->GetAt(i)[0]);
					mTmpVertices[i].normalandUV[0].uvSetName = t_uvSetName;
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < uvArrayIndicesCount; ++i)
				{

					t_uv.x = static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[1]);
					mTmpVertices[i].normalandUV[0].uv = t_uv;
					mTmpVertices[i].normalandUV[0].uvSetName = t_uvSetName;
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex:
			switch (uvReferenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect:
				for (int i = 0; i < polygonverticesCount; ++i)
				{

					t_uv.x = static_cast<float>(uvArray->GetAt(i)[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(i)[1]);
					mTmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]].uv = t_uv;
					mTmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]].uvSetName = t_uvSetName;
					++vertexRefCount[polygonvertices[i]];
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < uvArrayIndicesCount; ++i)
				{

					uvlArrayIndices[i] = uvArrayIndicesArray->GetAt(i);
					vertexIndices[i] = polygonvertices[i];

					t_uv.x = static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[1]);
					mTmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]].uv = t_uv;
					mTmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]].uvSetName = t_uvSetName;
					++vertexRefCount[polygonvertices[i]];
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygon:
			break;
		case fbxsdk::FbxLayerElement::eByEdge:
			break;
		case fbxsdk::FbxLayerElement::eAllSame:
			break;
		default:
			break;
		}
	}

	//END vertex uv load
}

void SkeltonStore(Fbx::FbxSkeleton& skl, const NodeTree& tree) {
	skl.name = tree.nodeName;
	skl.pos = ConvertXMFloat3ToXMFloat4(tree.translation);
	skl.rotation = ConvertXMFloat3ToXMFloat4(tree.rotation);
	skl.scale = ConvertXMFloat3ToXMFloat4(tree.scale);
};

void CreateSkeltonData(const NodeTree& skeltonTree,
	std::vector<unsigned int>& skeltonIndices, 
	std::vector<Fbx::FbxSkeleton>& skeltons,
	unsigned int& skeltonIndex)
{
	unsigned int parentIndex = 0;
	//親ボーンの処理
	Fbx::FbxSkeleton skl;
	SkeltonStore(skl, skeltonTree);
	skeltons[parentIndex = skeltonIndex++] = skl;

	//子ボーンの処理とインデックスの作成
	for (unsigned int j = 0; j < static_cast<unsigned int>(skeltonTree.children.size()); ++j)
	{
		//インデックスの作成
		skeltonIndices.push_back(parentIndex);
		skeltonIndices.push_back(skeltonIndex);

		CreateSkeltonData(skeltonTree.children[j], skeltonIndices, skeltons, skeltonIndex);
		//SkeltonStore(skl, skeltonTree.children[j]);
		//skeltons[skeltonIndex++] = skl;
	}
}

void FbxLoader::LoadBone(fbxsdk::FbxMesh* mesh)
{
	//START vertex bone weight load
	int t_skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int t_clusterCount = 0;

	fbxsdk::FbxCluster* t_cluster;
	int offset = 0;
	Fbx::TmpBone t_bone;
	fbxsdk::FbxAMatrix t_fbxMatrix;

	for (int i = 0; i < t_skinCount; ++i)
	{
		t_clusterCount = static_cast<fbxsdk::FbxSkin*> (mesh->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		fbxsdk::FbxSkin::EType skintype = static_cast<fbxsdk::FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin))->GetSkinningType();
		for (int j = 0; j < t_clusterCount; ++j, ++offset)
		{
			t_cluster = static_cast<fbxsdk::FbxSkin*> (mesh->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

			fbxsdk::FbxAMatrix transformMatrix = {};

			fbxsdk::FbxCluster::ELinkMode mode = t_cluster->GetLinkMode();

			int ctrlPointIndicesCount = t_cluster->GetControlPointIndicesCount();
			int* ctrlPointIndicesArray = t_cluster->GetControlPointIndices();
			double* ctrlPointWeightArray = t_cluster->GetControlPointWeights();

			Fbx::TmpWeight t_weight;

			t_bone.boneName = t_cluster->GetLink()->GetName();

			std::vector<int> t_vertexIndicesArray;
			t_vertexIndicesArray.reserve(ctrlPointIndicesCount);
			for (int k = 0; k < ctrlPointIndicesCount; ++k)
			{
				t_weight.weight = static_cast<float>(ctrlPointWeightArray[k]);
				t_weight.boneNo = offset;
				mTmpVertices[ctrlPointIndicesArray[k]].weights.push_back(t_weight);
				mTmpVertices[ctrlPointIndicesArray[k]].boneName.push_back(t_cluster->GetLink()->GetName());
				t_vertexIndicesArray.push_back(ctrlPointIndicesArray[k]);
			}

			StoreFbxMatrixToXMMatrix(transformMatrix, t_bone.matrix);

			mBones.push_back(t_bone);
		}
	}
	//EMD vertex bone weight load

	//START skelton Load
	fbxsdk::FbxNode* root = mScene->GetRootNode();
	NodeTree skeltonTree = {};
	std::vector<fbxsdk::FbxNode*> skeltonNode;
	StackSearchNode(root, fbxsdk::FbxNodeAttribute::eSkeleton, skeltonTree, [&skeltonNode](fbxsdk::FbxNode* node) {
		skeltonNode.push_back(node);
	});

	//ルートボーン用に一つ余分に確保
	mSkeltons.resize(skeltonNode.size() +  1);
	//基本的には2倍で足りると思う
	mSkeltonIndices.reserve(skeltonNode.size() * 2);

	unsigned int skeltonNum = static_cast<unsigned int>(skeltonNode.size());
	unsigned int skeltonIndex = 0;

	CreateSkeltonData(skeltonTree, mSkeltonIndices, mSkeltons, skeltonIndex);
	mSkeltonIndices.shrink_to_fit();
	//END skelton Load
}

//@param out mv: モデル頂点データ 
//@param tv: mvに入れる頂点データ
//@paran tmpvertexId: tvのnormalとuvのどのデータを使うかを示す添え字
void StoreTmpVertexToModelVertex(Fbx::FbxVertex& mv, const Fbx::TmpVertex& tv, int tmpvertexId)
{
	mv.pos = { tv.pos.x,tv.pos.y,tv.pos.z,1 };
	mv.normal = { tv.normalandUV[tmpvertexId].normal.x,tv.normalandUV[tmpvertexId].normal.y,tv.normalandUV[tmpvertexId].normal.z,1 };
	mv.texCoord = tv.normalandUV[tmpvertexId].uv;
	mv.boneIndex.resize(tv.weights.size());
	mv.boneWeight.resize(tv.weights.size());
	mv.boneName = tv.boneName;
	for (int i = 0; i < static_cast<int>(tv.weights.size()); ++i)
	{
		mv.boneIndex[i] = tv.weights[i].boneNo;
		mv.boneWeight[i] = tv.weights[i].weight;
	}
}

//頂点整合用関数
//@return 整合用頂点番号
int FbxLoader::CheckVertexDiff(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex)
{
	int refcount = mTmpVertices[vertexIndex].refcount++;
	if (refcount == 0)
	{
		StoreTmpVertexToModelVertex(vertex[vertexIndex], mTmpVertices[vertexIndex], refcount);
		return vertexIndex;
	}

	for (int i = 0; i < refcount; ++i)
	{
		if (mTmpVertices[vertexIndex].normalandUV[i] == mTmpVertices[vertexIndex].normalandUV[refcount])
		{
			mTmpVertices[vertexIndex].normalandUV[refcount].vertexNo = mTmpVertices[vertexIndex].normalandUV[i].vertexNo;
			StoreTmpVertexToModelVertex(vertex[mTmpVertices[vertexIndex].normalandUV[refcount].vertexNo], mTmpVertices[vertexIndex], refcount);
			return mTmpVertices[vertexIndex].normalandUV[i].vertexNo;
		}
	}

	Fbx::TmpVertex v;
	v.pos = mTmpVertices[vertexIndex].pos;
	v.normalandUV.push_back(mTmpVertices[vertexIndex].normalandUV[refcount]);
	v.refcount = 1;
	v.weights = mTmpVertices[vertexIndex].weights;
	v.normalandUV[0].vertexNo = static_cast<int>(mTmpVertices.size());
	v.boneName = mTmpVertices[vertexIndex].boneName;
	mTmpVertices[vertexIndex].normalandUV[refcount].vertexNo = static_cast<int>(mTmpVertices.size());
	mTmpVertices.push_back(v);
	StoreTmpVertexToModelVertex(vertex[v.normalandUV[0].vertexNo], v, 0);
	return v.normalandUV[0].vertexNo;
}

void FbxLoader::FixVertexInfo(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh* mesh)
{
	//START store vertex data

	int* indexBuffer = mesh->GetPolygonVertices();
	

	int polygoncount = mesh->GetPolygonCount();
	int polygonsize = 0;
	int error = 0;

	int polygonvertexcount = mesh->GetPolygonVertexCount();
	model->indexes.indexes.reserve(polygoncount * 6);
	model->vertexesInfo.vertexes.resize(polygoncount * 6);

	int index = indexBuffer[polygonvertexcount - 1];

	int indexplus1 = 0;
	int indexplus2 = 0;

	int vNum = 0;

	auto& mats = model->materials;
	for (unsigned int pi = 0, pvi = 0, materialId = 0, polygonCount = 0; static_cast<int>(pi) < polygoncount; ++pi) {
		polygonsize = mesh->GetPolygonSize(pi);
		switch (polygonsize)
		{
		case 3:
			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			mats[materialId].effectIndexNum += 3;
			break;
		case 4:
			model->indexes.indexes.push_back(indexplus1 = CheckVertexDiff(indexBuffer[pvi], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi + 1], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(indexplus2 = CheckVertexDiff(indexBuffer[pvi + 2], model->vertexesInfo.vertexes));


			model->indexes.indexes.push_back(indexplus1);

			model->indexes.indexes.push_back(indexplus2);

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi + 3], model->vertexesInfo.vertexes));

			mats[materialId].effectIndexNum += 6;

			pvi += 4;
			break;
		default:
			++error;
			break;
		}

		++polygonCount;
		if (mMaterialSets[materialId].polygonCount <= polygonCount)
		{
			++materialId;
			polygonCount = 0;
		}
	}

	if (error > 0)
	{
		MessageBox(NULL, L"エラポリあるんやけど", L"POLYGON ERROR", MB_OK);

	}

	std::vector<unsigned int>(model->indexes.indexes).swap(model->indexes.indexes);
	model->indexes.indexCount = static_cast<int>( model->indexes.indexes.size());
	
	auto beginitr = model->vertexesInfo.vertexes.begin();
	auto enditr = model->vertexesInfo.vertexes.end();
	int vindex = 0;
	while (beginitr != enditr)
	{
		if (vindex == mTmpVertices.size())
		{
			break;
		}
		vindex++;
		beginitr++;
	}
	model->vertexesInfo.vertexes.erase(beginitr, enditr);
	std::vector<Fbx::FbxVertex>(model->vertexesInfo.vertexes).swap(model->vertexesInfo.vertexes);
	//END store vertex data

	//START store bone data

	unsigned int size = static_cast<unsigned int>(mBones.size());
	model->bones.resize(size);

	int iCounter = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		model->bones[i].boneName = mBones[i].boneName;
		model->bones[i].index = i;
		model->bones[i].initMatrix = ConvertXMMATRIXToXMFloat4x4(mBones[i].matrix);
	}

	model->skeltons = std::move(mSkeltons);
	model->skeltonIndices = std::move(mSkeltonIndices);

	//END store bone data
}

void FbxLoader::StackSearchNode(fbxsdk::FbxNode* parent, fbxsdk::FbxNodeAttribute::EType searchtype, NodeTree& parentTree, std::function<void(fbxsdk::FbxNode*)> hitFunction)
{
	fbxsdk::FbxTime t = 0;

	int childNum = parent->GetChildCount();
	
	for (int i = 0, skipCount = 0; i < childNum; ++i)
	{
		fbxsdk::FbxNode* childNode = parent->GetChild(i);
		fbxsdk::FbxNodeAttribute* nodeAttribute = childNode->GetNodeAttribute();
		if (nodeAttribute == nullptr)
		{
			++skipCount;
			continue;
		}
		
		NodeTree childNodeTree;
		childNodeTree.nodeName = childNode->GetName();

		childNodeTree.attributeType = nodeAttribute->GetAttributeType();
		if (childNodeTree.attributeType == searchtype)
		{
			//http://help.autodesk.com/view/FBX/2019/ENU/?guid=FBX_Developer_Help_nodes_and_scene_graph_fbx_nodes_transformation_data_html
			auto t_translation = childNode->LclTranslation.Get();
			childNodeTree.translation = { static_cast<float>(t_translation.mData[0]), static_cast<float>(t_translation.mData[1]) , static_cast<float>(t_translation.mData[2]) };
			childNodeTree.translation += parentTree.translation;
			auto t_rotation = childNode->LclRotation.Get();
			childNodeTree.rotation = { static_cast<float>(t_rotation.mData[0]), static_cast<float>(t_rotation.mData[1]), static_cast<float>(t_rotation.mData[2]) };
			childNodeTree.rotation += parentTree.rotation;
			auto t_scale = childNode->LclScaling.Get();
			childNodeTree.scale = { static_cast<float>(t_scale.mData[0]), static_cast<float>(t_scale.mData[1]), static_cast<float>(t_scale.mData[2]) };
			childNodeTree.scale += parentTree.scale;

			parentTree.children.push_back(childNodeTree);
			hitFunction(childNode);
			StackSearchNode(childNode, searchtype, parentTree.children[i - skipCount], hitFunction);
		}
		else
		{
			++skipCount;
			StackSearchNode(childNode, searchtype, parentTree, hitFunction);
		}
	}
}

std::shared_ptr<Fbx::FbxModelData> FbxLoader::MainLoad(fbxsdk::FbxMesh* mesh, std::string path)
{
	std::shared_ptr<Fbx::FbxModelData> model= std::make_shared<Fbx::FbxModelData>();

	LoadVertexPosition(mesh);

	LoadVertexNormal(mesh);

	LoadVertexUV(mesh);

	LoadBone(mesh);

	LoadMatarial(model, mesh);

	FixVertexInfo(model, mesh);

	return model;
}

Fbx::FbxVertex CalculatoFbxVertexByFbxAMatrix(Fbx::FbxVertex v, fbxsdk::FbxAMatrix fmat)
{
	DirectX::XMMATRIX dmat;
	DirectX::XMMATRIX noMoveMat;
	Fbx::FbxVertex t_v;
	t_v = v;

	StoreFbxMatrixToXMMatrix(fmat, dmat);

	noMoveMat.r[3].m128_f32[0] = 0;
	noMoveMat.r[3].m128_f32[1] = 0;
	noMoveMat.r[3].m128_f32[2] = 0;

	DirectX::XMVECTOR t_pos;
	t_pos = DirectX::XMLoadFloat4(&v.pos);
	t_pos = DirectX::XMVector4Transform(t_pos, dmat);
	DirectX::XMStoreFloat4(&t_v.pos, t_pos);

	DirectX::XMVECTOR t_normal;
	t_normal = DirectX::XMLoadFloat4(&v.normal);
	t_normal = DirectX::XMVector4Transform(t_normal, dmat);
	DirectX::XMStoreFloat4(&t_v.normal, t_normal);

	return t_v;

}

/**
*	@param[in,out] resultDataこのデータの後ろに追加のデータを結合する
*	@param[in] connectData このデータをresultDataの後ろに結合する
*/
template <typename T>
void ConnectSTLVectorData(std::vector<T>& resultData,const std::vector<T>& connectData )
{
	unsigned int endNum = static_cast<unsigned int>(resultData.size());
	unsigned int addNum = static_cast<unsigned int>(connectData.size());
	resultData.reserve(endNum + addNum);
	for (auto& data : connectData)
	{
		resultData.push_back(data);
	}
}

/**
*	@param[in,out] resultIndices このデータの後ろに追加のデータを結合する
*	@param[in] connectIndices このデータをresultIndicesの後ろに結合する
*	@param[in] offset インデックスのオフセット量
*/
template <typename T>
void ConnectSTLVectorIndices(std::vector<T>& resultIndices, const std::vector<T>& connectIndices, const unsigned int offsetNum)
{
	unsigned int endNum = static_cast<unsigned int>(resultIndices.size());
	unsigned int addNum = static_cast<unsigned int>(connectIndices.size());
	resultIndices.reserve(endNum + addNum);
	for (auto& idx : connectIndices)
	{
		resultIndices.push_back(idx + offsetNum);
	}
}

std::shared_ptr<Fbx::FbxModelData> FbxLoader::ConnectMeshes(std::vector<std::shared_ptr<Fbx::FbxModelData>>& datas)
{
	std::shared_ptr<Fbx::FbxModelData> rtn = std::make_shared<Fbx::FbxModelData>();
	rtn->modelPath = datas[0]->modelPath;
	int endCount = 0;
	int addCount = 0;
	for (int i = 0;i < static_cast<int>(datas.size()); ++i)
	{
		endCount = static_cast<int>(rtn->indexes.indexes.size());
		addCount = static_cast<int>(datas[i]->indexes.indexes.size());
		rtn->indexes.indexCount = endCount + addCount;
		ConnectSTLVectorIndices(rtn->indexes.indexes, datas[i]->indexes.indexes, static_cast<unsigned int>(rtn->vertexesInfo.vertexes.size()));
		
		ConnectSTLVectorData(rtn->vertexesInfo.vertexes, datas[i]->vertexesInfo.vertexes);

		ConnectSTLVectorData(rtn->materials, datas[i]->materials);

		endCount = static_cast<int>(rtn->bones.size());
		addCount = static_cast<int>(datas[i]->bones.size());
		rtn->bones.reserve(endCount + addCount);
		for (auto& b : datas[i]->bones)
		{
			rtn->bones.push_back(b);
		}
	}

	return rtn;
}

void FbxLoader::ClearTmpInfo()
{
	mMeshDatas.clear();
	mMeshDatas.shrink_to_fit();
	mNodeDatas.clear();
	mNodeDatas.shrink_to_fit();
	mTmpVertices.clear();
	mTmpVertices.shrink_to_fit();
	mTmpIndexes.clear();
	mTmpIndexes.shrink_to_fit();
	mBones.clear();
	mBones.shrink_to_fit();
	mSkeltons.clear();
	mSkeltons.shrink_to_fit();
	mSkeltonIndices.clear();
	mSkeltonIndices.shrink_to_fit();
	mAnimCurves.clear();
	mAnimCurves.shrink_to_fit();
	mSkeletonMatrix.clear();
	mSkeletonMatrix.shrink_to_fit();
	fbxsdk::FbxAMatrix identity;
	identity.SetIdentity();
	mNodeTree.globalOffsetPosition = identity;
	mNodeTree.globalPosition = identity;
	mNodeTree.nodeName.clear();
	mNodeTree.nodeName.shrink_to_fit();
	mNodeTree.children.clear();
	mNodeTree.children.shrink_to_fit();
	mMaterialSets.clear();
	mMaterialSets.shrink_to_fit();
}

void FbxLoader::LoadAnimationMain(fbxsdk::FbxScene* scene, unsigned int meshId)
{
	std::vector<fbxsdk::FbxNode*> linkNode;
	std::vector<fbxsdk::FbxTime> times = ExtractingKeyFrames(scene, meshId, linkNode);

	int nodeIndex = mPose->Find(mNodeDatas[0]);

	fbxsdk::FbxMatrix poseMatrix = mPose->GetMatrix(nodeIndex);

	FbxAMatrix poseAMatrix;
	memcpy((double*)poseAMatrix, (double*)poseMatrix, sizeof(poseMatrix.mData));

	fbxsdk::FbxAMatrix globalOffsetPosition = poseAMatrix * mGeometryOffset;
	fbxsdk::FbxLongLong oneFrameValue = fbxsdk::FbxTime::GetOneFrameValue(fbxsdk::FbxTime::eFrames60);

	for (int i = 0; i < static_cast<int>(linkNode.size()); ++i)
	{
		int nodeIndex = mPose->Find(linkNode[i]);
		mSkeletonMatrix[i].animMatrix.resize(times.size());
		for (int j = 0; j < static_cast<int>(times.size()); ++j)
		{
			//FbxAMatrix globalPosition = mNodeDatas[meshId]->EvaluateGlobalTransform(times[j]);
			
			FbxAMatrix fmat = linkNode[i]->EvaluateGlobalTransform(times[j]);
			DirectX::XMMATRIX dmat;

			StoreFbxMatrixToXMMatrix(globalOffsetPosition.Inverse() * fmat, dmat);
			mSkeletonMatrix[i].animMatrix[j].matrix = dmat;
			mSkeletonMatrix[i].animMatrix[j].frame = times[j] / oneFrameValue;
		}
	}
}

std::vector<fbxsdk::FbxTime> FbxLoader::ExtractingKeyFrames(fbxsdk::FbxScene* scene, unsigned int meshId, std::vector<fbxsdk::FbxNode*>& linkNode)
{
	int animStack = scene->GetSrcObjectCount<FbxAnimStack>();

	std::vector<fbxsdk::FbxCluster*> clusters;
	
	for (int stack = 0; stack < animStack; ++stack)
	{
		FbxAnimStack* pAnimStack = scene->GetMember<FbxAnimStack>(stack);

		int numAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
		for (int layer = 0; layer < numAnimLayers; ++layer)
		{
			FbxAnimLayer* pAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(layer);

			unsigned int defomerCount = mMeshDatas[meshId]->GetDeformerCount(FbxDeformer::eSkin);
			for (unsigned int deformer = 0; deformer < defomerCount; ++deformer)
			{
				mAnimCurves.clear();
				fbxsdk::FbxSkin* pDeformer = (fbxsdk::FbxSkin*)mMeshDatas[meshId]->GetDeformer(deformer, FbxDeformer::eSkin);
				fbxsdk::FbxSkin::EType skintype = pDeformer->GetSkinningType();
				unsigned int clusterCount = pDeformer->GetClusterCount();
				mAnimCurves.reserve(clusterCount);
				linkNode.reserve(clusterCount);
				Fbx::AnimationData animData;

				for (unsigned int cluster = 0; cluster < clusterCount; ++cluster)
				{
					fbxsdk::FbxCluster* pCluster = pDeformer->GetCluster(cluster);
					clusters.push_back(pCluster);
					linkNode.push_back(pCluster->GetLink());

					animData.skeletonName = linkNode[cluster]->GetName();

					//transform
					animData.translationX.curve = linkNode[cluster]->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					LoadCurve(animData.translationX);


					animData.translationY.curve = linkNode[cluster]->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					LoadCurve(animData.translationY);


					animData.translationZ.curve = linkNode[cluster]->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					LoadCurve(animData.translationZ);


					//rotation
					animData.rotationX.curve = linkNode[cluster]->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					LoadCurve(animData.rotationX);


					animData.rotationY.curve = linkNode[cluster]->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					LoadCurve(animData.rotationY);


					animData.rotationZ.curve = linkNode[cluster]->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					LoadCurve(animData.rotationZ);


					//scale
					animData.scaleX.curve = linkNode[cluster]->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					LoadCurve(animData.scaleX);

					animData.scaleY.curve = linkNode[cluster]->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					LoadCurve(animData.scaleY);

					animData.scaleZ.curve = linkNode[cluster]->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					LoadCurve(animData.scaleZ);

					mAnimCurves.push_back(animData);
				}
			}
		}
	}

	std::vector<fbxsdk::FbxTime> times;

	mSkeletonMatrix.resize(mAnimCurves.size());
	unsigned int size = static_cast<int>(mSkeletonMatrix.size());

	int iCounter = 0;

	for (unsigned int i = 0; i < size; ++i)
	{
		StackAnimationTime(mAnimCurves[i].translationX.keys, times);
		StackAnimationTime(mAnimCurves[i].translationY.keys, times);
		StackAnimationTime(mAnimCurves[i].translationZ.keys, times);

		StackAnimationTime(mAnimCurves[i].rotationX.keys, times);
		StackAnimationTime(mAnimCurves[i].rotationY.keys, times);
		StackAnimationTime(mAnimCurves[i].rotationZ.keys, times);

		StackAnimationTime(mAnimCurves[i].scaleX.keys, times);
		StackAnimationTime(mAnimCurves[i].scaleY.keys, times);
		StackAnimationTime(mAnimCurves[i].scaleZ.keys, times);

		mSkeletonMatrix[i].skeletonName = mAnimCurves[i].skeletonName;
	}

	std::sort(times.begin(), times.end(), [](fbxsdk::FbxTime lval, fbxsdk::FbxTime rval) {return lval.Get() < rval.Get(); });

	return times;
}

void FbxLoader::LoadCurve(Fbx::AnimCurveData& curveData)
{
	if (curveData.curve == nullptr)
	{
		return;
	}
	int keyCount = curveData.curve->KeyGetCount();
	curveData.keys.Resize(keyCount);
	Fbx::AnimKeyData keydata;
	for (int key = 0; key < keyCount; ++key)
	{
		keydata.time = curveData.curve->KeyGetTime(key).Get();
		keydata.value = curveData.curve->KeyGetValue(key);
		curveData.keys[key] = (keydata);
	}
}

void FbxLoader::StackAnimationTime(const fbxsdk::FbxArray<Fbx::AnimKeyData>& data, std::vector<fbxsdk::FbxTime>& stack)
{
	for (int i = 0;i < data.Size(); ++i)
	{
		auto itr = std::find_if(stack.begin(), stack.end(), [&data,i](const fbxsdk::FbxTime& t) {return t.Get() == (data[i].time); });
		if (itr == stack.end())
		{
			stack.push_back(data[i].time);
		}
	}
}

void FbxLoader::DestroyScence(fbxsdk::FbxScene* scene)
{
	DestroyNode(scene->GetRootNode());
	scene->Destroy();
}

void FbxLoader::DestroyNode(fbxsdk::FbxNode* node)
{
	if (node == nullptr)
	{
return;
	}
	int count = node->GetChildCount();
	for (int i = 0; i < count; ++i)
	{
		DestroyNode(node->GetChild(i));
	}

	node->Destroy();
}

/**ローカル関数定義**/

void StoreDiffuse(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.diffuse.element.x = static_cast<float>(lambert->Diffuse.Get()[0]);
		material.diffuse.element.y = static_cast<float>(lambert->Diffuse.Get()[1]);
		material.diffuse.element.z = static_cast<float>(lambert->Diffuse.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.diffuse.element.x = static_cast<float>(phong->Diffuse.Get()[0]);
		material.diffuse.element.y = static_cast<float>(phong->Diffuse.Get()[1]);
		material.diffuse.element.z = static_cast<float>(phong->Diffuse.Get()[2]);
	}
}

void StoreDiffuseFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.diffuseFactor.element = static_cast<float>(lambert->DiffuseFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.diffuseFactor.element = static_cast<float>(phong->DiffuseFactor.Get());
	}
}

void StoreAmbient(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.ambient.element.x = static_cast<float>(lambert->Ambient.Get()[0]);
		material.ambient.element.y = static_cast<float>(lambert->Ambient.Get()[1]);
		material.ambient.element.z = static_cast<float>(lambert->Ambient.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.ambient.element.x = static_cast<float>(phong->Ambient.Get()[0]);
		material.ambient.element.y = static_cast<float>(phong->Ambient.Get()[1]);
		material.ambient.element.z = static_cast<float>(phong->Ambient.Get()[2]);
	}
}

void StoreAmbientFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.ambientFactor.element = static_cast<float>(lambert->DiffuseFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.ambientFactor.element = static_cast<float>(phong->DiffuseFactor.Get());
	}
}

void StoreSpecular(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		material.specular.element.x = 0.0f;
		material.specular.element.y = 0.0f;
		material.specular.element.z = 0.0f;
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.specular.element.x = static_cast<float>(phong->Specular.Get()[0]);
		material.specular.element.y = static_cast<float>(phong->Specular.Get()[1]);
		material.specular.element.z = static_cast<float>(phong->Specular.Get()[2]);
	}
}

void StoreSpecularFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceMaterial::ClassId))
	{
		material.specularFactor.element = 1.0f;
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.specularFactor.element = static_cast<float>(phong->SpecularFactor.Get());
	}
}

void StoreShininess(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceMaterial::ClassId))
	{
		material.shininess.element = 1.0f;
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.shininess.element = static_cast<float>(phong->Shininess.Get());
	}
}

void StoreEmissive(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.emissive.element.x = static_cast<float>(lambert->Emissive.Get()[0]);
		material.emissive.element.y = static_cast<float>(lambert->Emissive.Get()[1]);
		material.emissive.element.z = static_cast<float>(lambert->Emissive.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.emissive.element.x = static_cast<float>(phong->Emissive.Get()[0]);
		material.emissive.element.y = static_cast<float>(phong->Emissive.Get()[1]);
		material.emissive.element.z = static_cast<float>(phong->Emissive.Get()[2]);
	}
}

void StoreEmissiveFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.emissiveFactor.element = static_cast<float>(lambert->EmissiveFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.emissiveFactor.element = static_cast<float>(phong->EmissiveFactor.Get());
	}
}

void StoreTransparentColor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.transparentColor.element.x = static_cast<float>(lambert->TransparentColor.Get()[0]);
		material.transparentColor.element.y = static_cast<float>(lambert->TransparentColor.Get()[1]);
		material.transparentColor.element.z = static_cast<float>(lambert->TransparentColor.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.transparentColor.element.x = static_cast<float>(phong->TransparentColor.Get()[0]);
		material.transparentColor.element.y = static_cast<float>(phong->TransparentColor.Get()[1]);
		material.transparentColor.element.z = static_cast<float>(phong->TransparentColor.Get()[2]);
	}
}

void StoreTransparencyFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.transparencyFactor.element = static_cast<float>(lambert->TransparencyFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.transparencyFactor.element = static_cast<float>(phong->TransparencyFactor.Get());
	}
}

bool GetTexture(fbxsdk::FbxProperty& prop, Fbx::FbxTexturesSet& textures)
{
	bool rtn = false;
	Fbx::FbxTexturesSet texSet;
	Fbx::FbxTexture tex;

	int textureCount = prop.GetSrcObjectCount<fbxsdk::FbxTexture>();

	for (int i = 0; i < textureCount; ++i)
	{
		auto layeredTexture = prop.GetSrcObject<fbxsdk::FbxLayeredTexture>(i);
		if (layeredTexture != nullptr)
		{
			int inTextureNum = layeredTexture->GetSrcObjectCount<fbxsdk::FbxTexture>();
			for (int j = 0; j < inTextureNum; ++j)
			{
				auto fbxTex = layeredTexture->GetSrcObject<fbxsdk::FbxTexture>(j);
				auto fileTex = fbxsdk::FbxCast<FbxFileTexture>(fbxTex);
				tex.textureName = fileTex->GetName();
				tex.texturePath = fileTex->GetRelativeFileName();
				tex.uvSetName = fileTex->UVSet.Get().Buffer();
				texSet.push_back(tex);
				rtn = true;
			}
		}
		else
		{
			auto fbxTex = prop.GetSrcObject<fbxsdk::FbxTexture>(i);
			auto fileTex = fbxsdk::FbxCast<FbxFileTexture>(fbxTex);
			tex.textureName = fileTex->GetName();
			tex.texturePath = fileTex->GetRelativeFileName();
			fbxsdk::FbxString path = fileTex->GetRelativeFileName();
			tex.uvSetName = fileTex->UVSet.Get().Buffer();
			texSet.push_back(tex);
			rtn = true;
		}
	}

	textures = std::move(texSet);

	char doubleslash = '\\';
	if (textures.size() != 0)
	{
		for (bool convertFinish = false; !convertFinish;)
		{
			size_t size = textures.back().texturePath.find(doubleslash);
			if (size <= textures.back().texturePath.size())
			{
				textures.back().texturePath[size] = '/';
			}
			else
			{
				convertFinish = true;
			}
		}
	}

	return rtn;
}

/**ローカル関数定義終了**/

void FbxLoader::LoadMatarial(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh * mesh)
{
	//START store texture path

	const std::vector<std::string> TEXTURE_TABLE = {
		fbxsdk::FbxSurfaceMaterial::sDiffuse,
		fbxsdk::FbxSurfaceMaterial::sDiffuseFactor,
		fbxsdk::FbxSurfaceMaterial::sAmbient,
		fbxsdk::FbxSurfaceMaterial::sAmbientFactor,
		fbxsdk::FbxSurfaceMaterial::sSpecular,
		fbxsdk::FbxSurfaceMaterial::sSpecularFactor,
		fbxsdk::FbxSurfaceMaterial::sShininess,
		fbxsdk::FbxSurfaceMaterial::sEmissive,
		fbxsdk::FbxSurfaceMaterial::sEmissiveFactor,
		//fbxsdk::FbxSurfaceMaterial::sNormalMap,
		//fbxsdk::FbxSurfaceMaterial::sBump,
		fbxsdk::FbxSurfaceMaterial::sTransparentColor,
		fbxsdk::FbxSurfaceMaterial::sTransparencyFactor,
		//fbxsdk::FbxSurfaceMaterial::sReflection,
		//fbxsdk::FbxSurfaceMaterial::sReflectionFactor,
	};

	const std::vector <std::function<void(Fbx::FbxMaterial&, const fbxsdk::FbxSurfaceMaterial*)>> MATERIAL_TABLE = {
		StoreDiffuse,
		StoreDiffuseFactor,
		StoreAmbient,
		StoreAmbientFactor,
		StoreSpecular,
		StoreSpecularFactor,
		StoreShininess,
		StoreEmissive,
		StoreEmissiveFactor,
		StoreTransparentColor,
		StoreTransparencyFactor,
	};

	//マテリアルは１メッシュに一つの場合のみ対応
	int materialcount = mesh->GetNode()->GetMaterialCount();
	int elementMaterialCount = mesh->GetElementMaterialCount();

	mMaterialSets.resize(materialcount);
	auto polygonVerticesCount = mesh->GetPolygonVertexCount();
	auto polygonCount = mesh->GetPolygonCount();
	const int TRIANGEL_POLYGON_SIZE = 3;
	for(int element = 0; element < elementMaterialCount; ++element)
	{
		auto elementMaterial = mesh->GetElementMaterial(element);
		auto attribute = elementMaterial->GetMappingMode();

		if (attribute == fbxsdk::FbxLayerElement::eByPolygon)
		{
			fbxsdk::FbxLayerElementArrayTemplate<int>& polygonMaterialArray = elementMaterial->GetIndexArray();
			unsigned int indexCount = polygonMaterialArray.GetCount();
			Fbx::MaterialIndexSet matSet = { static_cast<unsigned int>(polygonMaterialArray.GetAt(0)), 0U };//ループの中でインクリメントするのでカウントは0初期化
			for (unsigned int i = 0; i < indexCount; ++i)
			{
				unsigned int currentRefMaterialId = polygonMaterialArray.GetAt(i);//同じマテリアルを複数回に分けて描画する場合には対応していない
				mMaterialSets[currentRefMaterialId].materialId = currentRefMaterialId;
				++mMaterialSets[currentRefMaterialId].polygonCount;
			}
		}
		else if (attribute == fbxsdk::FbxLayerElement::eAllSame)
		{
			mMaterialSets[0].polygonCount = polygonCount;
		}
	}

	model->materials.resize(materialcount);
	for (int i = 0; i < materialcount; ++i)
	{
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetMaterial(i);

		for (unsigned int j = 0; j < static_cast<unsigned int>(TEXTURE_TABLE.size());++j)
		{
			auto findPorp = surfaceMaterial->FindProperty(TEXTURE_TABLE[j].data());
			if (findPorp.IsValid())
			{
				Fbx::FbxTexturesSet texSet;
				if (GetTexture(findPorp, texSet))
				{
					model->materials[i].SetTexture((Fbx::FbxMaterial::eELEMENT_TYPE)(j), texSet);
				}
				else
				{
					MATERIAL_TABLE[j](model->materials[i], surfaceMaterial);
				}
			}
		}
	}

	//EBD store texture path
}
