#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxModelController.h"
#include "FbxModelDataCoverter.h"
#include "FbxMotionConverter.h"
#include "FbxMotionData.h"
#include "ShaderCompiler.h"
#include "RootSignatureObject.h"
#include "Dx12Ctrl.h"
#include "PipelineStateObject.h"
#include "XMFloatOperators.h"
#include "d3dx12.h"
#include "Util.h"
#include "RenderingPathManager.h"


#include <fbxsdk.h>
#include <memory>
#include <exception>
#include <cassert>
#include <algorithm>
#include <Windows.h>


std::unique_ptr<FbxLoader> FbxLoader::mInstance = nullptr;

const std::string FBX_SHADER_PATH = "DirectX12/FbxShader.hlsl";
const std::string FBX_VERTEXSHADER_NAME = "FbxVS";
const std::string FBX_PIXCELSHADER_NAME = "FbxPS";
const std::string FBX_GEOMETRYSHADER_NAME = "";
const std::string FBX_HULLSHADER_NAME = "";
const std::string FBX_DOMAINSHADER_NAME = "";

FbxLoader::FbxLoader():mModelConverter(new FbxModelDataConverter()),mMotionConverter(new FbxMotionConverter())
{
	mShader = ShaderCompiler::GetInstance()->CompileShader(
		FBX_SHADER_PATH,
		FBX_VERTEXSHADER_NAME,
		FBX_PIXCELSHADER_NAME,
		FBX_GEOMETRYSHADER_NAME,
		FBX_HULLSHADER_NAME,
		FBX_DOMAINSHADER_NAME,
		true);

	DX12CTRL_INSTANCE;
	CreateRootsignature(d12->GetDev());
	CreatePipelineState(d12->GetDev());
	mCmdList = RenderingPathManager::Instance()->GetRenderingPathCommandList(0);
}

void FbxLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;

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
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(mShader.pixcelShader.Get());
	gpsDesc.DS;
	gpsDesc.GS;
	gpsDesc.HS;

	mPipelinestate.reset(new PipelineStateObject(gpsDesc,dev));
}

void FbxLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mRootsignature.reset(new RootSignatureObject(mShader.rootSignature.Get(), dev));
	mRootsignature->GetRootSignature()->SetName(L"FbxRootSignature");
}

void FbxLoader::SetPipelineState(std::shared_ptr<PipelineStateObject>& Pipelinestate)
{
	mPipelinestate = Pipelinestate;
	mPipelinestate->GetPipelineState()->SetName(L"FbxPipelineState");
}

void FbxLoader::SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
}

FbxLoader::~FbxLoader()
{
	delete mModelConverter;
	delete mMotionConverter;
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

	fbxsdk::FbxAxisSystem directXAxisSys(fbxsdk::FbxAxisSystem::EUpVector::eYAxis,
		fbxsdk::FbxAxisSystem::EFrontVector::eParityOdd,
		fbxsdk::FbxAxisSystem::ECoordSystem::eLeftHanded);

	fbxsdk::FbxAxisSystem maxAxisSys(fbxsdk::FbxAxisSystem::Max);
	fbxsdk::FbxAxisSystem mayaAxisSysY(fbxsdk::FbxAxisSystem::MayaYUp);
	fbxsdk::FbxAxisSystem mayaAxisSysZ(fbxsdk::FbxAxisSystem::MayaZUp);

	fbxsdk::FbxAxisSystem system = maxAxisSys;

	//fbxsdk::FbxAxisSystem directXAxisSys(fbxsdk::FbxAxisSystem::eDirectX);

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
		rtn.reset(new FbxModelController(itr->second, Dx12Ctrl::Instance()->GetDev(), mCmdList, mPipelinestate, mRootsignature));
		return rtn;
	}

	if (!LoaderInitializie(modelPath))
	{
		return nullptr;
	}

	const int poseCount = mScene->GetPoseCount();
	if (poseCount > 1)
	{
		return nullptr;
	}
	else
	{
		mPose = mScene->GetPose(0);
	}

	//ルートノードのゲット
	fbxsdk::FbxNode* rootNode = mScene->GetRootNode();

	if (rootNode) {
		FbxAMatrix dummy;
		dummy.SetIdentity();
		NodeTree rNode;
		rNode.nodeName = rootNode->GetName();
		fbxsdk::FbxTime t = 0;
		rNode.globalPosition = GetGlobalPosition(rootNode,t,mPose,&dummy);
		rNode.globalOffsetPosition = rNode.globalPosition * GetGeometry(rootNode);
		mNodeTree = rNode;
		StackSearchNode(rootNode, FbxNodeAttribute::EType::eMesh, mNodeTree);
	}
	else {
		MessageBox(nullptr, L"Load Error <rootNode is null>", L"FBXModelLoader", MB_OK);
		return nullptr;
	}
	if (mMeshDatas.size() == 0)
	{
		MessageBox(nullptr, L"メッシュがないぞよ", L"FBXModelLoader", MB_OK);
		return rtn;
	}
	
	std::vector<Fbx::FbxModelData*> models(mMeshDatas.size());

	for (int i = 0; i < static_cast<int>(mMeshDatas.size()); i++)
	{
		//FbxAMatrix globalPosition = mNodeDatas[i]->EvaluateGlobalTransform();
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
	rtn.reset(new FbxModelController(model, Dx12Ctrl::Instance()->GetDev(), mCmdList, mPipelinestate, mRootsignature));
	
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
		rNode.globalPosition = GetGlobalPosition(rootNode, t, mPose, &dummy);
		rNode.globalOffsetPosition = rNode.globalPosition * GetGeometry(rootNode);
		mNodeTree = rNode;
		StackSearchNode(rootNode, FbxNodeAttribute::EType::eMesh, mNodeTree);
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
	for (int uvSetIndex = 0; uvSetIndex < t_UVSetNameList.GetCount(); ++uvSetIndex)
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
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < uvArrayIndicesCount; ++i)
				{

					t_uv.x = static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[1]);
					mTmpVertices[i].normalandUV[0].uv = t_uv;
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
					mTmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]++].uv = t_uv;
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
					mTmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]++].uv = t_uv;
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

void FbxLoader::LoadBone(fbxsdk::FbxMesh* mesh)
{
	//START vertex bone weight load
	int t_skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int t_clusterCount = 0;

	FbxCluster* t_cluster;
	int offset = 0;
	Fbx::TmpBone t_bone;
	fbxsdk::FbxAMatrix t_fbxMatrix;

	for (int i = 0; i < t_skinCount; ++i)
	{
		t_clusterCount = static_cast<fbxsdk::FbxSkin*> (mesh->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		//fbxsdk::FbxSkin::EType skintype = static_cast<fbxsdk::FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin))->GetSkinningType();
		for (int j = 0; j < t_clusterCount; ++j, ++offset)
		{
			t_cluster = static_cast<fbxsdk::FbxSkin*> (mesh->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

			fbxsdk::FbxAMatrix transformMatrix = {};
			const NodeTree* tree = GetNodeTree(t_cluster->GetLink()->GetName());
			fbxsdk::FbxTime t = 0;
			ComputeClusterDeformation(tree->globalOffsetPosition,mesh,t_cluster,transformMatrix,t,mPose);

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

			//OpenGLでの実装を参考にやっているので行列計算の順序が逆かもしれない
			//sceneをコンバートした結果行列を見た限りだとあっているように見えるので続行
			/*FbxAMatrix referenceGlobalInitPosition;
			t_cluster->GetTransformMatrix(referenceGlobalInitPosition);
			referenceGlobalInitPosition *= mGeometryOffset;

			t_cluster->GetTransformLinkMatrix(t_fbxMatrix);*/
			
			//FbxAMatrix relativeInitPosition = t_fbxMatrix.Inverse() * referenceGlobalInitPosition;

			//StoreFbxMatrixToXMMatrix(relativeInitPosition, t_bone.matrix);

			//for (int i = 0; i < 3; ++i)
			//{
			//	for (int j = 0; j < 4; ++j)
			//	{
			//		if (i == j)
			//		{
			//			t_fbxMatrix[i][j] = 1.0f;
			//		}
			//		else
			//		{
			//			t_fbxMatrix[i][j] = 0.0f;
			//		}
			//	}
			//}

			StoreFbxMatrixToXMMatrix(transformMatrix, t_bone.matrix);

			mBones.push_back(t_bone);
		}
	}
	//EMD vertex bone weight load
}

//@param _out_ mv: モデル頂点データ 
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

//テスト用ローカル関数
bool GetTexture(fbxsdk::FbxProperty& prop, Fbx::FbxModelData* model)
{
	bool rtn = false;
	Fbx::FbxTextureInfo texInfo;
	Fbx::FbxTexture tex;
	fbxsdk::FbxLayeredTexture* lTex;
	fbxsdk::FbxFileTexture* ftex;

	int layerdTextureCount = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();
	if (layerdTextureCount != 0)
	{
		for (int layer = 0; layer < layerdTextureCount; ++layer)
		{
			lTex = prop.GetSrcObject<fbxsdk::FbxLayeredTexture>(layer);
			int srcObjectCount = lTex->GetSrcObjectCount();
			for (int src = 0; src < srcObjectCount; ++src)
			{
				ftex = lTex->GetSrcObject<fbxsdk::FbxFileTexture>(src);
				tex.texturePath = ftex->GetRelativeFileName();
				tex.uvSetName = ftex->UVSet.Get().Buffer();
				tex.textureName = ftex->GetFileName();
				texInfo.textures.push_back(tex);
				rtn = true;
			}
			model->textures.push_back(texInfo);
		}
		return rtn;
	}

	char doubleslash = '\\';

	int count = prop.GetSrcObjectCount<fbxsdk::FbxFileTexture>();//おそらく0か1しかない？一応ループ処理はする
	for (int i = 0; i < count; ++i)
	{
		ftex = prop.GetSrcObject<fbxsdk::FbxFileTexture>(i);
		
		tex.texturePath = ftex->GetRelativeFileName();
		tex.uvSetName = ftex->UVSet.Get().Buffer();
		tex.textureName = ftex->GetFileName();
		texInfo.textures.push_back(tex);
		model->textures.push_back(texInfo);
		rtn = true;
	}

	if (model->textures.size() != 0)
	{
		for (bool convertFinish = false; !convertFinish;)
		{
			size_t size = model->textures.back().textures.back().texturePath.find(doubleslash);
			if (size <= model->textures.back().textures.back().texturePath.size())
			{
				model->textures.back().textures.back().texturePath[size] = '/';
			}
			else
			{
				convertFinish = true;
			}
		}
	}

	return rtn;
}

void FbxLoader::FixVertexInfo(Fbx::FbxModelData* model, fbxsdk::FbxMesh* mesh)
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
	for (int pi = 0, pvi = 0; pi < polygoncount; ++pi) {
		polygonsize = mesh->GetPolygonSize(pi);
		switch (polygonsize)
		{
		case 3:
			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			break;
		case 4:
			model->indexes.indexes.push_back(indexplus1 = CheckVertexDiff(indexBuffer[pvi], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi + 1], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(indexplus2 = CheckVertexDiff(indexBuffer[pvi + 2], model->vertexesInfo.vertexes));


			model->indexes.indexes.push_back(indexplus1);

			model->indexes.indexes.push_back(indexplus2);

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi + 3], model->vertexesInfo.vertexes));

			pvi += 4;
			break;
		default:
			++error;
			break;
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

	unsigned int size = mBones.size();
	model->bones.resize(size);

	int iCounter = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		model->bones[i].boneName = mBones[i].boneName;
		model->bones[i].index = i;
		StoreMatrixToFloat4x4(model->bones[i].initMatrix, mBones[i].matrix);
		////DirectX::XMStoreFloat4x4(&model->bones[i].initMatrix, mBones[i].matrix);
		//StoreMatrixToFloat4x4(model->bones[i].initMatrix, mBones[i].matrix);
	}
	//END store bone data

	//START store texture path

	int materialcount = mesh->GetNode()->GetMaterialCount();
	for (int i = 0; i < materialcount; ++i)
	{
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetMaterial(i);
		fbxsdk::FbxString string;
		fbxsdk::FbxProperty prop = surfaceMaterial->RootProperty;
		int testcount = 0;

		while (prop.IsValid())
		{
			string = prop.GetName();
			++testcount;
			GetTexture(prop, model);
			auto type = prop.GetValueInheritType();
			prop = surfaceMaterial->GetNextProperty(prop);
		}
	}

	//EBD store texture path
}

void FbxLoader::StackSearchNode(fbxsdk::FbxNode* parent, FbxNodeAttribute::EType searchtype, NodeTree& parentodetree)
{
	fbxsdk::FbxTime t = 0;

	int count = parent->GetChildCount();
	for (int i = 0; i < count; ++i)
	{
		NodeTree childnode;
		childnode.nodeName = parent->GetChild(i)->GetName();
		childnode.globalPosition = GetGlobalPosition(parent->GetChild(i), t, mPose, &parentodetree.globalPosition);
		childnode.globalOffsetPosition = childnode.globalPosition * GetGeometry(parent->GetChild(i));
		parentodetree.children.push_back(childnode);

		fbxsdk::FbxNodeAttribute* nodeAttribute = parent->GetChild(i)->GetNodeAttribute();
		if (nodeAttribute == nullptr) continue;
		
		FbxNodeAttribute::EType type = nodeAttribute->GetAttributeType();
		if (type == searchtype)
		{
			mMeshDatas.push_back(parent->GetChild(i)->GetMesh());
			mNodeDatas.push_back(parent->GetChild(i));
		}

		FbxString str = parent->GetChild(i)->GetName();
		FbxClassId id = nodeAttribute->GetClassId();

		StackSearchNode(parent->GetChild(i),searchtype, parentodetree.children[i]);
	}
}

Fbx::FbxModelData* FbxLoader::MainLoad(fbxsdk::FbxMesh* mesh, std::string path)
{
	Fbx::FbxModelData* model= new Fbx::FbxModelData();

	LoadVertexPosition(mesh);

	LoadVertexNormal(mesh);

	LoadVertexUV(mesh);

	LoadBone(mesh);

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

Fbx::FbxModelData* FbxLoader::ConnectMeshes(std::vector<Fbx::FbxModelData*>& datas)
{
	fbxsdk::FbxAMatrix fmat;

	Fbx::FbxModelData* rtn = new Fbx::FbxModelData();
	rtn->modelPath = datas[0]->modelPath;
	int endCount = 0;
	int addCount = 0;
	for (int i = 0;i < static_cast<int>(datas.size()); ++i)
	{
		endCount = static_cast<int>(rtn->indexes.indexes.size());
		addCount = static_cast<int>(datas[i]->indexes.indexes.size());
		rtn->indexes.indexes.reserve(endCount + addCount);
		rtn->indexes.indexCount = endCount + addCount;
		for (auto& idx : datas[i]->indexes.indexes)
		{
			rtn->indexes.indexes.push_back(idx + static_cast<unsigned int>(rtn->vertexesInfo.vertexes.size()));
		}
		
		fmat = mNodeDatas[i]->EvaluateGlobalTransform();
		endCount = static_cast<int>(rtn->vertexesInfo.vertexes.size());
		addCount = static_cast<int>(datas[i]->vertexesInfo.vertexes.size());
		rtn->vertexesInfo.vertexes.reserve(endCount + addCount);

		for (auto& v : datas[i]->vertexesInfo.vertexes)
		{
			/*Fbx::FbxVertex calv = CalculatoFbxVertexByFbxAMatrix(v, fmat);*/
			rtn->vertexesInfo.vertexes.push_back(v);
		}

		endCount = static_cast<int>(rtn->materials.size());
		addCount = static_cast<int>(datas[i]->materials.size());
		rtn->materials.reserve(endCount + addCount);
		for (auto& m : datas[i]->materials)
		{
			rtn->materials.push_back(m);
		}

		endCount = static_cast<int>(rtn->textures.size());
		addCount = static_cast<int>(datas[i]->textures.size());
		rtn->materials.reserve(endCount + addCount);
		for (auto& t : datas[i]->textures)
		{
			rtn->textures.push_back(t);
		}

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
	std::vector<fbxsdk::FbxMesh*>().swap(mMeshDatas);
	mNodeDatas.clear();
	std::vector<fbxsdk::FbxNode*>().swap(mNodeDatas);
	mTmpVertices.clear();
	std::vector<Fbx::TmpVertex>().swap(mTmpVertices);
	mTmpIndexes.clear();
	std::vector<int>().swap(mTmpIndexes);
	mBones.clear();
	std::vector<Fbx::TmpBone>().swap(mBones);
	mAnimCurves.clear();
	std::vector<Fbx::AnimationData>().swap(mAnimCurves);
	mSkeletonMatrix.clear();
	std::vector<Fbx::SkeletonAnimationData>().swap(mSkeletonMatrix);
	mNodeTree.children.clear();
	fbxsdk::FbxAMatrix identity;
	identity.SetIdentity();
	mNodeTree.globalOffsetPosition = identity;
	mNodeTree.globalPosition = identity;
	mNodeTree.nodeName.clear();
	std::string().swap(mNodeTree.nodeName);
	std::vector<NodeTree>().swap(mNodeTree.children);
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
				FbxSkin* pDeformer = (FbxSkin*)mMeshDatas[meshId]->GetDeformer(deformer, FbxDeformer::eSkin);
				fbxsdk::FbxSkin::EType skintype = pDeformer->GetSkinningType();
				unsigned int clusterCount = pDeformer->GetClusterCount();
				mAnimCurves.reserve(clusterCount);
				linkNode.reserve(clusterCount);
				Fbx::AnimationData animData;

				for (unsigned int cluster = 0; cluster < clusterCount; ++cluster)
				{
					FbxCluster* pCluster = pDeformer->GetCluster(cluster);
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
	unsigned int size = mSkeletonMatrix.size();

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


// global position at the current time.
fbxsdk::FbxAMatrix FbxLoader::GetGlobalPosition(fbxsdk::FbxNode* pNode, const fbxsdk::FbxTime& pTime, fbxsdk::FbxPose* pPose, fbxsdk::FbxAMatrix* pParentGlobalPosition)
{
	FbxAMatrix lGlobalPosition;
	bool        lPositionFound = false;

	if (pPose)
	{
		int lNodeIndex = pPose->Find(pNode);

		if (lNodeIndex > -1)
		{
			// The bind pose is always a global matrix.
			// If we have a rest pose, we need to check if it is
			// stored in global or local space.
			if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
			{
				lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
			}
			else
			{
				// We have a local matrix, we need to convert it to
				// a global space matrix.
				FbxAMatrix lParentGlobalPosition;

				if (pParentGlobalPosition)
				{
					lParentGlobalPosition = *pParentGlobalPosition;
				}
				else
				{
					if (pNode->GetParent())
					{
						lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
					}
				}

				FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
				lGlobalPosition = lParentGlobalPosition * lLocalPosition;
			}

			lPositionFound = true;
		}
	}

	if (!lPositionFound)
	{
		// There is no pose entry for that node, get the current global position instead.

		// Ideally this would use parent global position and local position to compute the global position.
		// Unfortunately the equation 
		//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
		// does not hold when inheritance type is other than "Parent" (RSrs).
		// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
		lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
	}

	return lGlobalPosition;
}

fbxsdk::FbxAMatrix FbxLoader::GetPoseMatrix(fbxsdk::FbxPose* pPose, int pNodeIndex)
{
	fbxsdk::FbxAMatrix lPoseMatrix;
	fbxsdk::FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

fbxsdk::FbxAMatrix FbxLoader::GetGeometry(fbxsdk::FbxNode* pNode)
{
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

void FbxLoader::DrawNodeRecursive(fbxsdk::FbxNode* pNode, fbxsdk::FbxTime& pTime, FbxAnimLayer* pAnimLayer,
	fbxsdk::FbxAMatrix& pParentGlobalPosition, fbxsdk::FbxPose* pPose)
{
	fbxsdk::FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);

	if (pNode->GetNodeAttribute())
	{
		// Geometry offset.
		// it is not inherited by the children.
		fbxsdk::FbxAMatrix lGeometryOffset = GetGeometry(pNode);
		fbxsdk::FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

		DrawNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose);
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		DrawNodeRecursive(pNode->GetChild(lChildIndex), pTime, pAnimLayer, lGlobalPosition, pPose);
	}
}

void FbxLoader::DrawNode(fbxsdk::FbxNode* pNode,
	fbxsdk::FbxTime& pTime,
	fbxsdk::FbxAnimLayer* pAnimLayer,
	fbxsdk::FbxAMatrix& pParentGlobalPosition,
	fbxsdk::FbxAMatrix& pGlobalPosition,
	fbxsdk::FbxPose* pPose)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute)
	{
		// All lights has been processed before the whole scene because they influence every geometry.
		if (lNodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMarker)
		{
			
		}
		else if (lNodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			//DrawSkeleton(pNode, pParentGlobalPosition, pGlobalPosition);
		}
		// NURBS and patch have been converted into triangluation meshes.
		else if (lNodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
		{
			//DrawMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose);
		}
	/*	else if (lNodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eCamera)
		{
			DrawCamera(pNode, pTime, pAnimLayer, pGlobalPosition);
		}
		else if (lNodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNull)
		{
			DrawNull(pGlobalPosition);
		}*/
	}
	else
	{
		// Draw a Null for nodes without attribute.
		//DrawNull(pGlobalPosition);
	}
}

void FbxLoader::ComputeClusterDeformation(const fbxsdk::FbxAMatrix& pGlobalPosition,
	fbxsdk::FbxMesh* pMesh,
	fbxsdk::FbxCluster* pCluster,
	fbxsdk::FbxAMatrix& pVertexTransformMatrix,
	fbxsdk::FbxTime pTime,
	fbxsdk::FbxPose* pPose)
{
	fbxsdk::FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

	fbxsdk::FbxAMatrix lReferenceGlobalInitPosition;
	fbxsdk::FbxAMatrix lReferenceGlobalCurrentPosition;
	fbxsdk::FbxAMatrix lAssociateGlobalInitPosition;
	fbxsdk::FbxAMatrix lAssociateGlobalCurrentPosition;
	fbxsdk::FbxAMatrix lClusterGlobalInitPosition;
	fbxsdk::FbxAMatrix lClusterGlobalCurrentPosition;

	fbxsdk::FbxAMatrix lReferenceGeometry;
	fbxsdk::FbxAMatrix lAssociateGeometry;
	fbxsdk::FbxAMatrix lClusterGeometry;

	fbxsdk::FbxAMatrix lClusterRelativeInitPosition;
	fbxsdk::FbxAMatrix lClusterRelativeCurrentPositionInverse;

	if (lClusterMode == FbxCluster::eAdditive && pCluster->GetAssociateModel())
	{
		pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);
		// Geometric transform of the model
		lAssociateGeometry = GetGeometry(pCluster->GetAssociateModel());
		lAssociateGlobalInitPosition *= lAssociateGeometry;
		lAssociateGlobalCurrentPosition = GetGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);

		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = GetGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;
		lReferenceGlobalCurrentPosition = pGlobalPosition;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		// Multiply lClusterGlobalInitPosition by Geometric Transformation
		lClusterGeometry = GetGeometry(pCluster->GetLink());
		lClusterGlobalInitPosition *= lClusterGeometry;
		lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the shift of the link relative to the reference.
		//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
		pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
			lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
	}
	else
	{
		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		lReferenceGlobalCurrentPosition = pGlobalPosition;
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = GetGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the initial position of the link relative to the reference.
		lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

		// Compute the current position of the link relative to the reference.
		lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

		// Compute the shift of the link relative to the reference.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
	}
}

void FbxLoader::ComputeLinearDeformation(fbxsdk::FbxAMatrix& pGlobalPosition,
	fbxsdk::FbxMesh* pMesh,
	fbxsdk::FbxTime& pTime,
	fbxsdk::FbxVector4* pVertexArray,
	fbxsdk::FbxPose* pPose)
{
	// All the links must have the same link mode.
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lVertexCount = pMesh->GetControlPointsCount();
	FbxAMatrix* lClusterDeformation = new FbxAMatrix[lVertexCount];
	memset(lClusterDeformation, 0, lVertexCount * sizeof(FbxAMatrix));

	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	if (lClusterMode == FbxCluster::eAdditive)
	{
		for (int i = 0; i < lVertexCount; ++i)
		{
			lClusterDeformation[i].SetIdentity();
		}
	}

	// For all skins and all clusters, accumulate their deformation and weight
	// on each vertices and store them in lClusterDeformation and lClusterWeight.
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int lSkinIndex = 0; lSkinIndex<lSkinCount; ++lSkinIndex)
	{
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);

		int lClusterCount = lSkinDeformer->GetClusterCount();
		for (int lClusterIndex = 0; lClusterIndex<lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink())
				continue;

			FbxAMatrix lVertexTransformMatrix;
			ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			for (int k = 0; k < lVertexIndexCount; ++k)
			{
				int lIndex = lCluster->GetControlPointIndices()[k];

				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (lIndex >= lVertexCount)
					continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
				{
					continue;
				}

				// Compute the influence of the link on the vertex.
				FbxAMatrix lInfluence = lVertexTransformMatrix;
				//MatrixScale(lInfluence, lWeight);
				lInfluence =lInfluence * lWeight;

				if (lClusterMode == FbxCluster::eAdditive)
				{
					// Multiply with the product of the deformations on the vertex.
					double value = 1.0 - lWeight;
					/*MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);*/
					lInfluence[0][0] += value;
					lInfluence[1][1] += value;
					lInfluence[2][2] += value;
					lInfluence[3][3] += value;
					lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];

					// Set the link to 1.0 just to know this vertex is influenced by a link.
					lClusterWeight[lIndex] = 1.0;
				}
				else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
				{
					// Add to the sum of the deformations on the vertex.
					//MatrixAdd(lClusterDeformation[lIndex], lInfluence);
					for (int i = 0; i < 4; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							lClusterDeformation[lIndex][i][j] += lInfluence[i][j];
						}
					}

					// Add to the sum of weights to either normalize or complete the vertex.
					lClusterWeight[lIndex] += lWeight;
				}
			}//For each vertex			
		}//lClusterCount
	}

	//Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
	for (int i = 0; i < lVertexCount; i++)
	{
		FbxVector4 lSrcVertex = pVertexArray[i];
		FbxVector4& lDstVertex = pVertexArray[i];
		double lWeight = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		if (lWeight != 0.0)
		{
			lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
			if (lClusterMode == FbxCluster::eNormalize)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				lDstVertex /= lWeight;
			}
			else if (lClusterMode == FbxCluster::eTotalOne)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				lSrcVertex *= (1.0 - lWeight);
				lDstVertex += lSrcVertex;
			}
		}
	}

	delete[] lClusterDeformation;
	delete[] lClusterWeight;
	
}

const NodeTree* FbxLoader::GetNodeTree(std::string nodename)
{
	return GetNodeTreeRecursive(nodename, mNodeTree);
}

const NodeTree* FbxLoader::GetNodeTreeRecursive(std::string nodeName, const NodeTree& nodeTree)
{

	if (nodeTree.nodeName == nodeName)
	{
		return &nodeTree;
	}

	for (auto& child : nodeTree.children)
	{
		const NodeTree* rtn = GetNodeTreeRecursive(nodeName, child);
		if (rtn != nullptr)
		{
			return rtn;
		}
	}
	
	return nullptr;
}