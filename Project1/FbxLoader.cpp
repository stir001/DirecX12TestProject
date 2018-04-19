#include "FbxLoader.h"
#include "FbxModel.h"
#include <KLib.h>
#include <fbxsdk.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <exception>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <thread>

FbxLoader* FbxLoader::instance = nullptr;

Fbx::FbxModel* FbxLoader::LoadMesh(std::string modelPath)
{
	//SDK�}�l�[�W���̍쐬
	fbxsdk::FbxManager* manager = fbxsdk::FbxManager::Create();

	//IO�Z�b�e�B���O�̐ݒ�
	fbxsdk::FbxIOSettings* ioSettings = fbxsdk::FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ioSettings);
	fbxsdk::FbxImporter*importer = fbxsdk::FbxImporter::Create(manager, "importer");
	//�C���|�[�^�[������
	if (!importer->Initialize(modelPath.c_str(), -1, manager->GetIOSettings())) {
		MessageBox(nullptr, L"Load Error <Imprter is null>", L"FBXModelLoader", MB_OK);
		return nullptr;
	}
	//�V�[���̍쐬
	fbxsdk::FbxScene* scene = fbxsdk::FbxScene::Create(manager, "Scene");

	//���[�h�����t�@�C�����V�[���ɃC���|�[�g
	importer->Import(scene);
	//�C���|�[�^�[�̔j���iFbxScene���ݒ肳���ƃC���|�[�^�[�����S�ɔj���ł���炵���j
	importer->Destroy();

	//���[�g�m�[�h�̃Q�b�g
	fbxsdk::FbxNode* rootNode = scene->GetRootNode();
	if (rootNode) {
		StackMeshNode(rootNode);
	}
	else {
		MessageBox(nullptr, L"Load Error <rootNode is null>", L"FBXModelLoader", MB_OK);
		return nullptr;
	}


	
	std::vector<Fbx::FbxModel*> models(m_meshDatas.size());

	for (int i = 0; i < m_meshDatas.size(); i++)
	{
		models[i] = MainLoad(m_meshDatas[i], modelPath);
	}

	//LoadAnimation(scene);


	//scene->Destroy();//�G���[���ł�
	//manager->Destroy();//�G���[���o��

	//�ύX�K�v
	return models[0];
}

void FbxLoader::LoadAnimation(fbxsdk::FbxScene* scene)
{
	int animStackCount = scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
	for (int stackIndex = 0; stackIndex < animStackCount; ++stackIndex)
	{
		fbxsdk::FbxAnimStack* t_animStack = scene->GetSrcObject<fbxsdk::FbxAnimStack>(stackIndex);
		int animLayerCount = t_animStack->GetMemberCount<fbxsdk::FbxAnimLayer>();
		fbxsdk::FbxAnimLayer* t_animLayer;
		for (int layer = 0; layer < animLayerCount; ++layer)
		{
			t_animLayer = t_animStack->GetSrcObject<fbxsdk::FbxAnimLayer>(layer);
			int nodecount = t_animLayer->GetSrcObjectCount<fbxsdk::FbxAnimCurveNode>();
			fbxsdk::FbxAnimCurveNode* t_animCurveNode;
			for (int curve = 0; curve < nodecount; ++curve)
			{
				t_animCurveNode = t_animLayer->GetSrcObject<fbxsdk::FbxAnimCurveNode>(curve);
				
			}
			int a = 0;
		}
	}
}

Fbx::FbxModel * FbxLoader::LoadAnimation(std::string modelPath)
{
	return nullptr;
}

std::string FbxLoader::GetRelativeTexturePath(std::string modelPath, std::string texturePath)
{
	size_t pathIndex1 = modelPath.rfind('\\');
	size_t pathIndex2 = modelPath.rfind('/');
	size_t pathIndex = min(pathIndex1, pathIndex2);
	std::string textureTempPath = modelPath.substr(0, pathIndex);
	textureTempPath += '\\';
	textureTempPath += texturePath;

	return textureTempPath;
}

//Fbx::FbxIndexes FbxLoader::IndexLoad(fbxsdk::FbxMesh* mesh)
//{
//	Fbx::FbxIndexes indexes;
//	int* indexBuffer = mesh->GetPolygonVertices();
//	
//	indexes.indexCount = mesh->GetPolygonVertexCount();
//	indexes.indexes.resize(indexes.indexCount);
//	resource.indexes.resize(indexes.indexCount);
//	for (int i = 0; i < indexes.indexCount;i++) {
//		indexes.indexes[i] = indexBuffer[i];
//		resource.indexes[i] = indexBuffer[i];
//	}
//	return std::move(indexes);
//}

void FbxLoader::IndexLoad(fbxsdk::FbxMesh * mesh, Fbx::VertexResource& resource)
{
	Fbx::FbxIndexes indexes;
	int* indexBuffer = mesh->GetPolygonVertices();

	//std::vector<KLib::Math::Vector4> quadPolygons;
	//std::vector<KLib::Math::Vector3> triPolygons;

	int polygoncount = mesh->GetPolygonCount();
	int polygonsize = 0;
	int error = 0;
	
	//KLib::Math::Vector3 tmptrip;
	//KLib::Math::Vector4 tmpquadp;

	int polygonvertexcount = mesh->GetPolygonVertexCount();
	m_tmpIndexes.reserve(polygoncount * 6);
	
	for (int pi = 0,pvi = 0; pi < polygoncount && pvi < polygonvertexcount; ++pi) {
		polygonsize = mesh->GetPolygonSize(pi);
		switch (polygonsize)
		{
		case 3:
			m_tmpIndexes.push_back(indexBuffer[pvi++]);
			
			m_tmpIndexes.push_back(indexBuffer[pvi++]);
			
			m_tmpIndexes.push_back(indexBuffer[pvi++]);
			
			break;
		case 4:
			m_tmpIndexes.push_back(indexBuffer[pvi]);

			m_tmpIndexes.push_back(indexBuffer[pvi + 1]);

			m_tmpIndexes.push_back(indexBuffer[pvi + 2]);


			m_tmpIndexes.push_back(indexBuffer[pvi + 1]);

			m_tmpIndexes.push_back(indexBuffer[pvi + 3]);

			m_tmpIndexes.push_back(indexBuffer[pvi + 2]);
	
			pvi += 4;
			//triPolygons.push_back(tmptrip);
			break;
		default:
			++error;
			break;
		}
	}
	std::vector<int>(m_tmpIndexes).swap(m_tmpIndexes);
	//for (int i = 0; i <resource.indexes.size(); i++) {
	//	
	//	resource.indexes[i] = indexBuffer[i];
	//}
}

//Fbx::FbxVertexesInfo FbxLoader::VertexLoad(fbxsdk::FbxMesh* mesh)
//{
//
//	Fbx::FbxVertexesInfo vertexes;
//	vertexes.polygonCount = mesh->GetPolygonCount();
//	vertexes.vertexCount = mesh->GetControlPointsCount(); // �R���g���[���|�C���g�̐��@���Ƃŕ���UV�l�������_�𕪗����邽�ߕύX����
//
//	resource.pos.resize(mesh->GetControlPointsCount());
//	fbxsdk::FbxVector4* vertexBuffer = mesh->GetControlPoints();
//
//
//	vertexes.vertexes.resize(vertexes.vertexCount);
//	for (int i = 0; i < vertexes.vertexCount; i++) {
//		vertexes.vertexes[i].pos.x = (float)vertexBuffer[i][0];
//		vertexes.vertexes[i].pos.y = (float)vertexBuffer[i][1];
//		vertexes.vertexes[i].pos.z = (float)vertexBuffer[i][2];
//
//		resource.pos[i].x = (float)vertexBuffer[i][0];
//		resource.pos[i].y = (float)vertexBuffer[i][1];
//		resource.pos[i].z = (float)vertexBuffer[i][2];
//	}
//
//	return std::move(vertexes);
//
//}

void FbxLoader::VertexLoad(fbxsdk::FbxMesh * mesh, Fbx::VertexResource& resource)
{

	resource.pos.resize(mesh->GetControlPointsCount());
	fbxsdk::FbxVector4* vertexBuffer = mesh->GetControlPoints();
	for (int i = 0; i < resource.pos.size(); i++) {

		resource.pos[i].x = (float)vertexBuffer[i][0];
		resource.pos[i].y = (float)vertexBuffer[i][1];
		resource.pos[i].z = (float)vertexBuffer[i][2];
	}
}

//�e�X�g�p���[�J���֐�
void StoreFbxMatrixToKLibMatrix(const fbxsdk::FbxAMatrix& fmat, KLib::Math::Matrix& kmat)
{
	kmat._11 = static_cast<float>(fmat.Get(0, 0)); kmat._12 = static_cast<float>(fmat.Get(0, 1)); kmat._13 = static_cast<float>(fmat.Get(0, 2)); kmat._14 = static_cast<float>(fmat.Get(0, 3));
	kmat._21 = static_cast<float>(fmat.Get(1, 0)); kmat._22 = static_cast<float>(fmat.Get(1, 1)); kmat._23 = static_cast<float>(fmat.Get(1, 2)); kmat._24 = static_cast<float>(fmat.Get(1, 3));
	kmat._31 = static_cast<float>(fmat.Get(2, 0)); kmat._32 = static_cast<float>(fmat.Get(2, 1)); kmat._33 = static_cast<float>(fmat.Get(2, 2)); kmat._34 = static_cast<float>(fmat.Get(2, 3));
	kmat._41 = static_cast<float>(fmat.Get(3, 0)); kmat._42 = static_cast<float>(fmat.Get(3, 1)); kmat._43 = static_cast<float>(fmat.Get(3, 2)); kmat._44 = static_cast<float>(fmat.Get(3, 3));
}


void FbxLoader::TmpVertexInfo(fbxsdk::FbxMesh* mesh)
{
	//START vertex position load
	int ctlPointcount = mesh->GetControlPointsCount();
	m_tmpVertices.resize(ctlPointcount);

	fbxsdk::FbxVector4* vertexBuffer = mesh->GetControlPoints();
	for (int i = 0; i < ctlPointcount; i++)
	{
		m_tmpVertices[i].pos.x = static_cast<float>(vertexBuffer[i][0]);
		m_tmpVertices[i].pos.y = static_cast<float>(vertexBuffer[i][1]);
		m_tmpVertices[i].pos.z = static_cast<float>(vertexBuffer[i][2]);
	}
	//END vertex position load 

	int* polygonvertices = mesh->GetPolygonVertices();
	int polygonverticesCount = mesh->GetPolygonVertexCount();

	//START vertex normal load
	int normallayercount = mesh->GetElementNormalCount();
	for (int layer = 0; layer < normallayercount; layer++)
	{
		fbxsdk::FbxGeometryElementNormal* normalElement = mesh->GetElementNormal(layer);
		fbxsdk::FbxGeometryElement::EReferenceMode referenceMode = normalElement->GetReferenceMode();
		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4> normalArray = normalElement->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int> normalArrayIndicesArray = normalElement->GetIndexArray();//normalArray�ɑΉ�����Index���i�[���Ă���z��

		fbxsdk::FbxGeometryElement::EMappingMode mappingMode = normalElement->GetMappingMode();

		fbxsdk::FbxVector4 t_normal;
		Fbx::TmpNormalUV nluv;
		
		int eIndexToDirectCount = 0;
		switch (mappingMode)
		{
		case fbxsdk::FbxLayerElement::eNone://�}�b�s���O������@�Ή��ł��Ȃ��ӂ������
			break;
		case fbxsdk::FbxLayerElement::eByControlPoint://�R���g���[���|�C���g��1��1�Ή�
			switch (referenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect://eByControllPoint�̏ꍇ�̓C���_�̏��Ԃɒ�`����΂����͂�
				for (int i = 0; i < m_tmpVertices.size(); ++i)
				{
					m_tmpVertices[i].normalandUV.resize(1);
					t_normal = normalArray.GetAt(i);
					m_tmpVertices[i].normalandUV[0].normal.x = static_cast<float>(t_normal[0]);
					m_tmpVertices[i].normalandUV[0].normal.y = static_cast<float>(t_normal[1]);
					m_tmpVertices[i].normalandUV[0].normal.z = static_cast<float>(t_normal[2]);
					m_tmpVertices[i].normalandUV[0].vertexNo = i;
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect://�����炭����͗��Ȃ��H
				eIndexToDirectCount = normalArrayIndicesArray.GetCount();
				for (int i = 0; i < eIndexToDirectCount; ++i)
				{
					t_normal = normalArray.GetAt(normalArrayIndicesArray.GetAt(i));
					nluv.normal.x = static_cast<float>(t_normal[0]);
					nluv.normal.y = static_cast<float>(t_normal[1]);
					nluv.normal.z = static_cast<float>(t_normal[2]);
					nluv.uv.x = 0;
					nluv.uv.y = 0;
					nluv.vertexNo = i;
					m_tmpVertices[i].normalandUV.push_back(nluv);
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex://�|���S�����Ƃ̃o�[�e�b�N�X����1��t1�Ή��@�C���f�b�N�X�z�񕪗p�ӂ���Ă���H
			switch (referenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect://eByPolygonVertex�̏ꍇ��PolygonVertex�̏��ԂŃo�[�b�ăN�X���`����΂����͂�
				for (int i = 0; i < polygonverticesCount; ++i)
				{
					t_normal = normalArray.GetAt(i);
					nluv.normal.x = static_cast<float>(t_normal[0]);
					nluv.normal.y = static_cast<float>(t_normal[1]);
					nluv.normal.z = static_cast<float>(t_normal[2]);
					nluv.vertexNo = i;
					m_tmpVertices[polygonvertices[i]].normalandUV.push_back(nluv);
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < polygonverticesCount; ++i)
				{
					t_normal = normalArray.GetAt(normalArrayIndicesArray.GetAt(i));
					nluv.normal.x = static_cast<float>(t_normal[0]);
					nluv.normal.y = static_cast<float>(t_normal[1]);
					nluv.normal.z = static_cast<float>(t_normal[2]);
					nluv.uv.x = 0;
					nluv.uv.y = 0;
					nluv.vertexNo = polygonvertices[i];
					m_tmpVertices[polygonvertices[i]].normalandUV.push_back(nluv);
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygon://�|���S�����Ƃɋ���
			break;
		case fbxsdk::FbxLayerElement::eByEdge://�悭�킩���
			break;
		case fbxsdk::FbxLayerElement::eAllSame://�S���ꏏ
			break;
		default:
			break;
		}
	}
	//END vertex normal load

	std::vector<int> vertexRefCount(m_tmpVertices.size());
	std::fill(vertexRefCount.begin(), vertexRefCount.end(), 0);

	//START vertex uv load

	fbxsdk::FbxStringList t_UVSetNameList;
	mesh->GetUVSetNames(t_UVSetNameList);
	for (int uvSetIndex = 0; uvSetIndex < t_UVSetNameList.GetCount(); ++uvSetIndex)
	{
		const char* t_uvSetName = t_UVSetNameList.GetStringAt(uvSetIndex);
		const FbxGeometryElementUV* t_uvElement = mesh->GetElementUV(t_uvSetName);

		if (t_uvElement == nullptr) continue;

		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector2> uvArray = t_uvElement->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int> uvArrayIndicesArray = t_uvElement->GetIndexArray();
		int uvArrayIndicesCount = uvArrayIndicesArray.GetCount();

		fbxsdk::FbxGeometryElement::EReferenceMode uvReferenceMode = t_uvElement->GetReferenceMode();
		fbxsdk::FbxGeometryElement::EMappingMode uvMappingMode = t_uvElement->GetMappingMode();

		KLib::Math::Vector2 t_uv;

		switch (uvMappingMode)
		{
		case fbxsdk::FbxLayerElement::eNone:
			break;
		case fbxsdk::FbxLayerElement::eByControlPoint:
			switch (uvReferenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect:
				for (int i = 0; i < m_tmpVertices.size(); ++i)
				{
					m_tmpVertices[i].normalandUV[0].uv.x = static_cast<float>(uvArray.GetAt(i)[0]);
					m_tmpVertices[i].normalandUV[0].uv.y = static_cast<float>(uvArray.GetAt(i)[0]);
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < uvArrayIndicesCount; ++i)
				{
					t_uv.x = static_cast<float>(uvArray.GetAt(uvArrayIndicesArray[i])[0]);
					t_uv.y = static_cast<float>(uvArray.GetAt(uvArrayIndicesArray[i])[1]);
					m_tmpVertices[i].normalandUV[0].uv = t_uv;
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
					t_uv.x = static_cast<float>(uvArray.GetAt(i)[0]);
					t_uv.y = static_cast<float>(uvArray.GetAt(i)[1]);
					m_tmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]++].uv = t_uv;
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < uvArrayIndicesCount; ++i)
				{
					t_uv.x = static_cast<float>(uvArray.GetAt(uvArrayIndicesArray[i])[0]);
					t_uv.y = static_cast<float>(uvArray.GetAt(uvArrayIndicesArray[i])[1]);
					m_tmpVertices[polygonvertices[i]].normalandUV[vertexRefCount[polygonvertices[i]]++].uv = t_uv;
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
		for (int j = 0; j < t_clusterCount; ++j,++offset)
		{
			t_cluster = static_cast<fbxsdk::FbxSkin*> (mesh->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

			fbxsdk::FbxCluster::ELinkMode mode = t_cluster->GetLinkMode();

			int ctrlPointIndicesCount = t_cluster->GetControlPointIndicesCount();
			int* ctrlPointIndicesArray = t_cluster->GetControlPointIndices();
			double* ctrlPointWeightArray = t_cluster->GetControlPointWeights();

			Fbx::TmpWeight t_weight;

			for (int k = 0; k < ctrlPointIndicesCount; ++k)
			{
				t_weight.weight = static_cast<float>(ctrlPointWeightArray[k]);
				t_weight.boneNo = offset;
				m_tmpVertices[ctrlPointIndicesArray[k]].weights.push_back(t_weight);
			}
			
			t_fbxMatrix = t_cluster->GetTransformMatrix(t_fbxMatrix);
			StoreFbxMatrixToKLibMatrix(t_fbxMatrix, t_bone.matrix);

			t_bone.boneName = t_cluster->GetName();

			m_bones.push_back(t_bone);
		}
	}
	//EMD vertex bone weight load
}

//@param _out_ mv: ���f�����_�f�[�^ 
//@param tv: mv�ɓ���钸�_�f�[�^
//@paran tmpvertexId: tv��normal��uv�̂ǂ̃f�[�^���g�����������Y����
void StoreTmpVertexToModelVertex(Fbx::FbxVertex& mv, const Fbx::TmpVertex& tv, int tmpvertexId)
{
	mv.pos = tv.pos;
	mv.normal = tv.normalandUV[tmpvertexId].normal;
	mv.texCoord = tv.normalandUV[tmpvertexId].uv;
	mv.boneIndex.resize(tv.weights.size());
	mv.boneWeight.resize(tv.weights.size());
	for (int i = 0; i < tv.weights.size(); ++i)
	{
		mv.boneIndex[i] = tv.weights[i].boneNo;
		mv.boneWeight[i] = tv.weights[i].weight;
	}
}

//���_�����p�֐�
//@return �����p���_�ԍ�
int FbxLoader::CheckVertexDiff(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex)
{
	int refcount = m_tmpVertices[vertexIndex].refcount++;
	if (refcount == 0)
	{
		StoreTmpVertexToModelVertex(vertex[vertexIndex], m_tmpVertices[vertexIndex], refcount);
		return vertexIndex;
	}

	for (int i = 0; i < refcount; ++i)
	{
		if (m_tmpVertices[vertexIndex].normalandUV[i] == m_tmpVertices[vertexIndex].normalandUV[refcount])
		{
			m_tmpVertices[vertexIndex].normalandUV[refcount].vertexNo = m_tmpVertices[vertexIndex].normalandUV[i].vertexNo;
			StoreTmpVertexToModelVertex(vertex[vertexIndex], m_tmpVertices[vertexIndex], refcount);
			return m_tmpVertices[vertexIndex].normalandUV[i].vertexNo;
		}
	}

	Fbx::TmpVertex v;
	v.pos = m_tmpVertices[vertexIndex].pos;
	v.normalandUV.push_back(m_tmpVertices[vertexIndex].normalandUV[refcount]);
	v.refcount = 0;
	v.weights = m_tmpVertices[vertexIndex].weights;
	v.normalandUV[0].vertexNo = static_cast<int>(m_tmpVertices.size());
	m_tmpVertices.push_back(v);
	StoreTmpVertexToModelVertex(vertex[v.normalandUV[0].vertexNo], v, 0);
	return v.normalandUV[0].vertexNo;
}

//�e�X�g�p���[�J���֐�
bool GetTexture(fbxsdk::FbxProperty& prop, Fbx::FbxModel* model)
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
				tex.textureName = ftex->GetName();
				texInfo.textures.push_back(tex);
				rtn = true;
			}
			model->textures.push_back(texInfo);
		}
		return rtn;
	}

	int count = prop.GetSrcObjectCount<fbxsdk::FbxFileTexture>();//�����炭0��1�����Ȃ��H�ꉞ���[�v�����͂���
	for (int i = 0; i < count; ++i)
	{
		ftex = prop.GetSrcObject<fbxsdk::FbxFileTexture>(i);
		
		tex.texturePath = ftex->GetRelativeFileName();
		tex.uvSetName = ftex->UVSet.Get().Buffer();
		tex.textureName = ftex->GetName();
		texInfo.textures.push_back(tex);
		model->textures.push_back(texInfo);
		rtn = true;
	}

	return rtn;
}

void FbxLoader::FixVertexInfo(Fbx::FbxModel* model, fbxsdk::FbxMesh* mesh)
{
	//START store vertex data
	int* indexBuffer = mesh->GetPolygonVertices();

	int polygoncount = mesh->GetPolygonCount();
	int polygonsize = 0;
	int error = 0;

	int polygonvertexcount = mesh->GetPolygonVertexCount();
	model->indexes.indexes.reserve(polygoncount * 6);
	model->vertexesInfo.vertexes.resize(polygoncount * 6);

	int indexplus1 = 0;
	int indexplus2 = 0;

	int vNum = 0;
	for (int pi = 0, pvi = 0; pi < polygoncount && pvi < polygonvertexcount; ++pi) {
		polygonsize = mesh->GetPolygonSize(pi);
		switch (polygonsize)
		{
		case 3:
			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi++], model->vertexesInfo.vertexes));

			break;
		case 4:
			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(indexplus1 = CheckVertexDiff(indexBuffer[pvi + 1], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(indexplus2 = CheckVertexDiff(indexBuffer[pvi + 2], model->vertexesInfo.vertexes));


			model->indexes.indexes.push_back(indexplus1);

			model->indexes.indexes.push_back(CheckVertexDiff(indexBuffer[pvi + 3], model->vertexesInfo.vertexes));

			model->indexes.indexes.push_back(indexplus2);

			pvi += 4;
			break;
		default:
			++error;
			break;
		}
	}

	std::vector<int>(model->indexes.indexes).swap(model->indexes.indexes);
	
	auto& beginitr = model->vertexesInfo.vertexes.begin();
	auto& enditr = model->vertexesInfo.vertexes.end();
	int vindex = 0;
	while (beginitr != enditr)
	{
		if (vindex == m_tmpVertices.size())
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
	model->bones.resize(m_bones.size());
	for (int i = 0; i < m_bones.size(); ++i)
	{
		model->bones[i].boneName = m_bones[i].boneName;
		model->bones[i].index = i;
		model->bones[i].initMatrix = m_bones[i].matrix;
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
}

//std::vector<KLib::Math::Vector3> FbxLoader::NormalLoad(fbxsdk::FbxMesh * mesh, Fbx::FbxIndexes& indexes)
//{
//	int normalLayerCount = mesh->GetElementNormalCount();
//	std::vector<KLib::Math::Vector3> normals;
//	for (int layer = 0; layer < normalLayerCount; layer++) {
//		//�G�������g�̎擾
//		fbxsdk::FbxGeometryElementNormal* normalElement = mesh->GetElementNormal(layer);
//		
//		
//		//uv�����擾
//		int normalDirectCount = normalElement->GetDirectArray().GetCount();
//		int normalIndexCount = normalElement->GetIndexArray().GetCount();
//		int size = normalDirectCount > normalIndexCount ? normalDirectCount : normalIndexCount;
//		
//		//�}�b�s���O���[�h�̎擾(C++17�̐V�@�\���g���Ă��܂��j
//		switch (fbxsdk::FbxGeometryElement::EMappingMode mappingMode = normalElement->GetMappingMode())
//		{
//		case fbxsdk::FbxLayerElement::eByPolygonVertex:
//			//���t�@�����X���[�h�̎擾
//			switch (fbxsdk::FbxGeometryElement::EReferenceMode referenceMode = normalElement->GetReferenceMode())
//			{
//			case fbxsdk::FbxLayerElement::eDirect:
//				normals.resize(size);
//				for (int i = 0; i < size; i++) {
//					normals[i].x = (float)normalElement->GetDirectArray().GetAt(i)[0];
//					normals[i].y = (float)normalElement->GetDirectArray().GetAt(i)[1];
//					normals[i].z = (float)normalElement->GetDirectArray().GetAt(i)[2];
//
//					resource.normal[i] = normals[i];
//				}
//				break;
//			case fbxsdk::FbxLayerElement::eIndexToDirect:
//				fbxsdk::FbxLayerElementArrayTemplate<int>* normalIndex = &normalElement->GetIndexArray();
//				resource.uvIndexList.resize(size);
//				for (int i = 0; i < size; i++) {
//					normals[i].x = (float)normalElement->GetDirectArray().GetAt(normalIndex->GetAt(i))[0];
//					normals[i].y = (float)normalElement->GetDirectArray().GetAt(normalIndex->GetAt(i))[1];
//					normals[i].z = (float)normalElement->GetDirectArray().GetAt(normalIndex->GetAt(i))[2];
//					resource.normalIndexList[i] = normalIndex->GetAt(i);
//					resource.normal[i] = normals[i];
//
//				}
//				break;
//
//			}
//			break;
//		case fbxsdk::FbxLayerElement::eByControlPoint:
//			break;
//		}
//	}
//	return std::move(normals);
//}

void FbxLoader::NormalLoad(fbxsdk::FbxMesh * mesh, Fbx::VertexResource& resource)
{
	int normalLayerCount = mesh->GetElementNormalCount();
	std::vector<KLib::Math::Vector3> normals;
	for (int layer = 0; layer < normalLayerCount; layer++) {
		//�G�������g�̎擾
		fbxsdk::FbxGeometryElementNormal* normalElement = mesh->GetElementNormal(layer);

		//uv�����擾
		int normalDirectCount = normalElement->GetDirectArray().GetCount();
		int normalIndexCount = normalElement->GetIndexArray().GetCount();
		int size = normalDirectCount > normalIndexCount ? normalDirectCount : normalIndexCount;
		resource.normal.resize(size);
		resource.normalIndexList.resize(size);
		//�}�b�s���O���[�h�̎擾(C++17�̐V�@�\���g���Ă��܂��j
		switch (fbxsdk::FbxGeometryElement::EMappingMode mappingMode = normalElement->GetMappingMode())
		{
		case fbxsdk::FbxLayerElement::eByPolygonVertex:
			//���t�@�����X���[�h�̎擾
			switch (fbxsdk::FbxGeometryElement::EReferenceMode referenceMode = normalElement->GetReferenceMode())
			{
			case fbxsdk::FbxLayerElement::eDirect:
				normals.resize(size);
				for (int i = 0; i < size; i++) {
					resource.normal[i].x = (float)normalElement->GetDirectArray().GetAt(i)[0];
					resource.normal[i].y = (float)normalElement->GetDirectArray().GetAt(i)[1];
					resource.normal[i].z = (float)normalElement->GetDirectArray().GetAt(i)[2];
				}
				break;
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				fbxsdk::FbxLayerElementArrayTemplate<int>* normalIndex = &normalElement->GetIndexArray();
				resource.uvIndexList.resize(size);
				for (int i = 0; i < size; i++) {
					resource.normal[i].x = (float)normalElement->GetDirectArray().GetAt(normalIndex->GetAt(i))[0];
					resource.normal[i].y = (float)normalElement->GetDirectArray().GetAt(normalIndex->GetAt(i))[1];
					resource.normal[i].z = (float)normalElement->GetDirectArray().GetAt(normalIndex->GetAt(i))[2];
					resource.normalIndexList[i] = normalIndex->GetAt(i);
					
				}
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByControlPoint:
			break;
		}
	}
}

//std::vector<KLib::Math::Vector2> FbxLoader::UVLoad(fbxsdk::FbxMesh * mesh, std::vector<int>& uvIndexList , Fbx::FbxIndexes& indexes)
//{
//	int uvLayerCount = mesh->GetElementUVCount();
//	std::vector<KLib::Math::Vector2> texcoord;
//	for (int layer = 0; layer < uvLayerCount; layer++) {
//		fbxsdk::FbxGeometryElementUV* uv = mesh->GetElementUV(layer);
//
//		//uv�����擾
//		int uvDirectCount = uv->GetDirectArray().GetCount();
//		int uvIndexCount = uv->GetIndexArray().GetCount();
//		int size = uvDirectCount > uvIndexCount ? uvDirectCount : uvIndexCount;
//		
//		//�}�s���O���[�h
//		switch (fbxsdk::FbxGeometryElement::EMappingMode mappingMode = uv->GetMappingMode())
//		{
//		case fbxsdk::FbxLayerElement::eByPolygonVertex:
//			//���t�@�����X���[�h
//			switch (fbxsdk::FbxGeometryElement::EReferenceMode referenceMode = uv->GetReferenceMode())
//			{
//
//			case fbxsdk::FbxLayerElement::eDirect:
//				texcoord.resize(size);
//				for (int j = 0; j < size; j++) {
//					texcoord[j].x = (float)uv->GetDirectArray().GetAt(j)[0];
//					texcoord[j].y = (float)uv->GetDirectArray().GetAt(j)[1];
//					resource.texCoord[j] = texcoord[j];
//				}
//				break;
//			case fbxsdk::FbxLayerElement::eIndex:
//			case fbxsdk::FbxLayerElement::eIndexToDirect:
//				fbxsdk::FbxLayerElementArrayTemplate<int>* uvIndex = &uv->GetIndexArray();
//				texcoord.resize(size);
//				resource.uvIndexList.resize(size);
//				uvIndexList.resize(size);
//				for (int j = 0; j < size; j++) {
//					texcoord[j].x = (float)uv->GetDirectArray().GetAt(uvIndex->GetAt(j))[0];
//					texcoord[j].y = (float)uv->GetDirectArray().GetAt(uvIndex->GetAt(j))[1];
//					resource.uvIndexList[j] = uvIndex->GetAt(j);
//					uvIndexList[j] = uvIndex->GetAt(j);
//					resource.texCoord[j] = texcoord[j];
//					
//				}
//				break;
//			}
//			break;
//		}
//	}
//	return std::move(texcoord);
//}

void FbxLoader::UVLoad(fbxsdk::FbxMesh * mesh, Fbx::VertexResource& resource)
{
	int uvLayerCount = mesh->GetElementUVCount();
	for (int layer = 0; layer < uvLayerCount; layer++) {
		fbxsdk::FbxGeometryElementUV* uv = mesh->GetElementUV(layer);

		//uv�����擾
		int uvDirectCount = uv->GetDirectArray().GetCount();
		int uvIndexCount = uv->GetIndexArray().GetCount();
		int size = uvDirectCount > uvIndexCount ? uvDirectCount : uvIndexCount;
		resource.texCoord.resize(size);
		//�}�s���O���[�h
		switch (fbxsdk::FbxGeometryElement::EMappingMode mappingMode = uv->GetMappingMode())
		{
		case fbxsdk::FbxLayerElement::eByPolygonVertex:
			//���t�@�����X���[�h
			switch (fbxsdk::FbxGeometryElement::EReferenceMode referenceMode = uv->GetReferenceMode())
			{
			case fbxsdk::FbxLayerElement::eDirect:
				for (int j = 0; j < size; j++) {
					resource.texCoord[j].x = (float)uv->GetDirectArray().GetAt(j)[0];
					resource.texCoord[j].y = (float)uv->GetDirectArray().GetAt(j)[1];
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				fbxsdk::FbxLayerElementArrayTemplate<int>* uvIndex = &uv->GetIndexArray();
				resource.uvIndexList.resize(size);
				for (int j = 0; j < size; j++) {
					resource.texCoord[j].x = (float)uv->GetDirectArray().GetAt(uvIndex->GetAt(j))[0];
					resource.texCoord[j].y = (float)uv->GetDirectArray().GetAt(uvIndex->GetAt(j))[1];
					resource.uvIndexList[j] = uvIndex->GetAt(j);
				}
				break;
			}
			break;
		}
	}
}

std::vector<Fbx::FbxMaterial> FbxLoader::MaterialLoad(fbxsdk::FbxMesh* mesh)
{
	std::vector<Fbx::FbxMaterial> materials;
	if (mesh->GetNode() == 0) {
		return std::move(materials);
	}
	int materialNum = mesh->GetNode()->GetMaterialCount();
	
	if (materialNum == 0) {
		return std::move(materials);
	}
	materials.resize(materialNum);

	int materialElementCount = mesh->GetElementMaterialCount();
	std::vector<int> materialIndexCount(materialNum);
	for (int i = 0; i < materialNum; i++) {
		fbxsdk::FbxGeometryElementMaterial* elementMaterial = mesh->GetElementMaterial(i);
		//���݂���ꍇ
		if (elementMaterial) {
			int indexArrayCount = 0;
			
			switch (fbxsdk::FbxGeometryElement::EMappingMode mappingMode = elementMaterial->GetMappingMode())
			{
			case fbxsdk::FbxLayerElement::eByPolygonVertex:
				//���t�@�����X���[�h
				switch (fbxsdk::FbxGeometryElement::EReferenceMode referenceMode = elementMaterial->GetReferenceMode())
				{

				case fbxsdk::FbxLayerElement::eIndex:
					indexArrayCount = elementMaterial->GetIndexArray().GetCount();
					for (int index = 0; index < indexArrayCount; index++) {
						materialIndexCount[elementMaterial->GetIndexArray().GetAt(index)]++;
					}
					break;
				case fbxsdk::FbxLayerElement::eDirect:
					
					break;
				case fbxsdk::FbxLayerElement::eIndexToDirect:
					indexArrayCount = elementMaterial->GetIndexArray().GetCount();
					for (int index = 0; index < indexArrayCount; index++) {
						materialIndexCount[elementMaterial->GetIndexArray().GetAt(index)]++;
					}
					break;
				}
				break;
			}
		}
	}
	
	for (int i = 0; i < materialNum; i++) {
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetMaterial(i);
		if (surfaceMaterial != 0) {
			//����܎g�������Ȃ����ǃ_�E���L���X�g�ŏ�������B
			if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId)) {
				fbxsdk::FbxSurfaceLambert* lambert = static_cast<fbxsdk::FbxSurfaceLambert*>(surfaceMaterial);

				materials[i].ambient.x = (float)lambert->Ambient.Get()[0];
				materials[i].ambient.y = (float)lambert->Ambient.Get()[1];
				materials[i].ambient.z = (float)lambert->Ambient.Get()[2];

				materials[i].ambientFactor = (float)lambert->AmbientFactor.Get();

				materials[i].diffuse.x = (float)lambert->Diffuse.Get()[0];
				materials[i].diffuse.y = (float)lambert->Diffuse.Get()[1];
				materials[i].diffuse.z = (float)lambert->Diffuse.Get()[2];

				materials[i].diffuseFactor = (float)lambert->DiffuseFactor.Get();

				materials[i].emissive.x = (float)lambert->Emissive.Get()[0];
				materials[i].emissive.y = (float)lambert->Emissive.Get()[1];
				materials[i].emissive.z = (float)lambert->Emissive.Get()[2];

				materials[i].emissiveFactor = (float)lambert->EmissiveFactor.Get();

				materials[i].bump.x = (float)lambert->Bump.Get()[0];
				materials[i].bump.y = (float)lambert->Bump.Get()[1];
				materials[i].bump.z = (float)lambert->Bump.Get()[2];

				materials[i].bumpFactor = (float)lambert->BumpFactor.Get();

				materials[i].transparent.x = (float)lambert->TransparentColor.Get()[0];
				materials[i].transparent.y = (float)lambert->TransparentColor.Get()[1];
				materials[i].transparent.z = (float)lambert->TransparentColor.Get()[2];

				materials[i].transparentFactor = (float)lambert->TransparencyFactor.Get();
			}
			if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId)) {
				fbxsdk::FbxSurfacePhong* phong = static_cast<fbxsdk::FbxSurfacePhong*>(surfaceMaterial);
				materials[i].specular.x = (float)phong->Specular.Get()[0];
				materials[i].specular.y = (float)phong->Specular.Get()[1];
				materials[i].specular.z = (float)phong->Specular.Get()[2];

				materials[i].specularFactor = (float)phong->SpecularFactor.Get();

				materials[i].shininess = (float)phong->Shininess.Get();

				materials[i].reflection.x = (float)phong->Reflection.Get()[0];
				materials[i].reflection.y = (float)phong->Reflection.Get()[1];
				materials[i].reflection.z = (float)phong->Reflection.Get()[2];

				materials[i].reflectionPower = (float)phong->ReflectionFactor.Get();

			}
			materials[i].surfaceVertex = materialIndexCount[i]*3;
		}
	}
	return std::move(materials);
}


std::vector<Fbx::FbxTextureInfo> FbxLoader::TextureLoad(fbxsdk::FbxMesh* mesh,std::string path)
{
	std::vector<Fbx::FbxTextureInfo> textures;
	if (mesh->GetNode() == 0) {
		return std::move(textures);
	}
	int materialNum = mesh->GetNode()->GetMaterialCount();

	if (materialNum == 0) {
		return std::move(textures);
	}
	//�}�e���A�����Ԃ��
	textures.resize(materialNum);
	for (int i = 0; i < materialNum; i++) {
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetMaterial(i);
		if (surfaceMaterial != 0) {
			Fbx::FbxTexture texRes;
			//�f�B�t���[�Y�v���p�e�B������
			fbxsdk::FbxProperty prop = surfaceMaterial->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);

			

			///�e�X�g�R�[�h
			//fbxsdk::FbxString string = prop.GetName();
			//fbxsdk::FbxProperty testProp = mesh->RootProperty;
			//int testcount = 0;
	
			//while (testProp.IsValid())
			//{
			//	string = testProp.GetName();
			//	++testcount;
			//	GetTexture(testProp);
			//	auto type = testProp.GetValueInheritType();
			//	testProp = surfaceMaterial->GetNextProperty(testProp);
			//}
			///�e�X�g�R�[�h�I��

			//�v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N
			int layeredTextureCount = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();

			//�������C���[�h�e�N�X�`��������Ȃ��
			if (layeredTextureCount > 0) {
				for (int j = 0; j < layeredTextureCount; j++) {
					//�e�N�X�`�����擾
					fbxsdk::FbxLayeredTexture* texture = prop.GetSrcObject<fbxsdk::FbxLayeredTexture>(j);
					int textureCount = texture->GetSrcObjectCount<fbxsdk::FbxFileTexture>();
					for (int k = 0; k < textureCount; k++) {
						fbxsdk::FbxFileTexture* layeredTexture = texture->GetSrcObject<fbxsdk::FbxFileTexture>(k);
						if (layeredTexture) {

							//�e�N�X�`�����̎擾
							std::string textureName = layeredTexture->GetRelativeFileName();
							//UVset���̎擾
							std::string uvSetName = layeredTexture->UVSet.Get().Buffer();

							texRes.texturePath = GetRelativeTexturePath(path, textureName);;
							texRes.uvSetName = uvSetName;
							texRes.textureName = texture->GetName();
							texRes.textureCount = 0;
							textures[i].textures.push_back(texRes);
						}
					}
				}
			}
			//�ʏ�e�N�X�`���̃��[�h
			else {
				//�ʏ�e�N�X�`�����������邩
				int standardTextureCount = prop.GetSrcObjectCount<fbxsdk::FbxTexture>();
				for (int j = 0; j < standardTextureCount; j++) {
					//j�Ԗڂ̃e�N�X�`���I�u�W�F�N�g���擾
					fbxsdk::FbxFileTexture* texture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(prop.GetSrcObject<fbxsdk::FbxFileTexture>(j));
					//�e�N�X�`�����̎擾
					if (texture) {
						//std::string textureName = texture->GetName();
						std::string textureName = texture->GetRelativeFileName();
						//UVset���̎擾
						std::string uvSetName = texture->UVSet.Get().Buffer();
						Fbx::FbxTexture texRes;
						texRes.texturePath = GetRelativeTexturePath(path, textureName);
						texRes.textureName = texture->GetName();
						texRes.textureCount = 0;
						texRes.uvSetName = uvSetName;
						textures[i].textures.push_back(texRes);

					}
				}
			}
		}
	}
	return std::move(textures);
}

std::vector<Fbx::FbxBoneInfo> FbxLoader::SkeltonLoad(fbxsdk::FbxMesh* mesh, std::string path)
{
	std::vector<Fbx::FbxBoneInfo> bones;

	int skinCount = mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
	if (skinCount <= 0) {
		return std::move(bones);
	}
	
	
	for (int i = 0; i < skinCount; i++) {
		fbxsdk::FbxSkin* skins = static_cast<fbxsdk::FbxSkin*>(mesh->GetDeformer(i, fbxsdk::FbxDeformer::eSkin));
		int clusterCount = skins->GetClusterCount();
		bones.resize(clusterCount);
		for (int j = 0; j < clusterCount; j++) {

			fbxsdk::FbxCluster* cluster = skins->GetCluster(j);

			int affectedPointNum = cluster->GetControlPointIndicesCount();
			int* pointArray = cluster->GetControlPointIndices();
			double* weightArray = cluster->GetControlPointWeights();

			fbxsdk::FbxAMatrix initMat;
			cluster->GetTransformLinkMatrix(initMat);

			Fbx::FbxBoneInfo bone;
			bone.initMatrix = KLib::Math::Matrix(
				static_cast<float>(initMat.Get(0, 0)), static_cast<float>(initMat.Get(0, 1)), static_cast<float>(initMat.Get(0, 2)), static_cast<float>(initMat.Get(0, 3)),
				static_cast<float>(initMat.Get(1, 0)), static_cast<float>(initMat.Get(1, 1)), static_cast<float>(initMat.Get(1, 2)), static_cast<float>(initMat.Get(1, 3)),
				static_cast<float>(initMat.Get(2, 0)), static_cast<float>(initMat.Get(2, 1)), static_cast<float>(initMat.Get(2, 2)), static_cast<float>(initMat.Get(2, 3)),
				static_cast<float>(initMat.Get(3, 0)), static_cast<float>(initMat.Get(3, 1)), static_cast<float>(initMat.Get(3, 2)), static_cast<float>(initMat.Get(3, 3))
			);
			bone.index = j;
			bone.boneName = cluster->GetName();
			
			for (int k = 0; k < affectedPointNum; k++) {
				int index = pointArray[k];
				bone.affectedVertexIndex.push_back(index);

				float weight = (float)weightArray[k];
				bone.affectedVertexWeight.push_back(weight);
			}
			bones[j] = bone;
		}
	}
	return std::move(bones);
}

void FbxLoader::FixSkelton(std::vector<Fbx::FbxBoneInfo>& bones)
{
	//////�R���o�[�g
	////for (auto& bone : bones) {
	////	for (int i = 0; i < bone.affectedVertexIndex.size(); i++) {
	////		model->m_vertexesInfo.vertexes[bone.affectedVertexIndex[i]].boneWeight.push_back(bone.affectedVertexWeight[i]);
	////		model->m_vertexesInfo.vertexes[bone.affectedVertexIndex[i]].boneIndex.push_back(bone.index);
	////	}
	////}
	////return;
}

fbxsdk::FbxNode * FbxLoader::SerchNode(fbxsdk::FbxNode * node, fbxsdk::FbxNodeAttribute::EType type)
{
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		if (node->GetChild(i)->GetNodeAttribute()->GetAttributeType() == type) {
			return node->GetChild(i);
		}
	};
	for (int i = 0; i < node->GetChildCount(); i++) {
		return SerchNode(node->GetChild(i), type);
	}

	return nullptr;
}

void FbxLoader::StackMeshNode(fbxsdk::FbxNode* parent)
{
	for (int i = 0; i < parent->GetChildCount(); ++i)
	{
		fbxsdk::FbxNodeAttribute* nodeAttribute = parent->GetChild(i)->GetNodeAttribute();
		if (nodeAttribute == nullptr) continue;
		
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			m_meshDatas.push_back(parent->GetChild(i)->GetMesh());
		}

		StackMeshNode(parent->GetChild(i));
	}
}

//void FbxLoader::FixVertex(Fbx::FbxVertexesInfo & vertexes, Fbx::FbxIndexes & index, std::vector<KLib::Math::Vector3>& normals, std::vector<KLib::Math::Vector2>& uvSet, std::vector<int>& uvIndexList)
//{
//	
//	//���_�Ɠ��������m�ۂ��ꂽ�x�N�^�z��B���̒��_������Q�Ƃ��ꂽ������������B
//	{
//		std::vector<std::map<int, std::pair<KLib::Math::Vector3,KLib::Math::Vector2>>> indexRefs(vertexes.vertexes.size());
//		//���_�C���f�b�N�X�������񂷁B���̃X�e�b�v�ł͓������_������Q�Ƃ��ꂽ�����m�F����B�����Q��ȏ�Q�Ƃ����Ȃ�A��̃X�e�b�v�Łu���ȏ�Q�Ƃ���ĂāA�ŏ��ɎQ�Ƃ��ꂽUV�ƈႤUV�l���g���v���_��T��
//
//
//		for (int i = 0; i < index.indexCount; i++) {
//
//
//			if (indexRefs[index.indexes[i]].size() == 0) {
//
//				indexRefs[index.indexes[i]][i] = std::pair<KLib::Math::Vector3, KLib::Math::Vector2>(normals[index.indexes[i]],uvSet[uvIndexList[i]]);
//				//����ɎQ�Ƃ���钸�_�͂������ꂿ�Ⴆ
//				vertexes.vertexes[index.indexes[i]].texCoord = uvSet[uvIndexList[i]];
//				vertexes.vertexes[index.indexes[i]].normal = normals[i];
//				continue;
//			}
//
//			for (auto itr = indexRefs[index.indexes[i]].begin(); itr != indexRefs[index.indexes[i]].end();itr++) {
//				//����ɑ�������l�Ȃ�Ώ������X�L�b�v�B���̗v�f�ֈړ�
//				if (vertexes.vertexes[index.indexes[i]].texCoord == uvSet[uvIndexList[i]] && vertexes.vertexes[index.indexes[i]].normal == normals[i]) {
//					continue;
//				}
//				else if(itr->second.second == uvSet[uvIndexList[i]] && itr->second.first == normals[i]){//��������UV�l&&�����@�������v�f�y�A���������Ȃ�
//					
//					//�C���f�b�N�X��������
//					//�ŏ��ɂ���l�̃L�[�l�͂��̒��_�̈ʒu�ɏ��������Ă���i���������C���f�b�N�X���ɂȂ��l�ɂ��Ă���̂ő�����邱�Ƃ͂Ȃ��ƐM�������j
//					index.indexes[i] = itr->first;
//
//				}
//				else if(itr->second.second != uvSet[uvIndexList[i]] && itr->second.first != normals[i]) {//���������ꂪ�V�����Q�Ə��i�@����UV���Ⴄ�j�Ȃ�Β��_�����R�s�[
//					
//					Fbx::FbxVertex vertexTemp = vertexes.vertexes[index.indexes[i]];
//
//					//���_���̃P�c�ɉ������ށBUV�l����������
//					vertexTemp.texCoord = uvSet[uvIndexList[i]];
//					vertexTemp.normal = normals[i];
//					vertexes.vertexes.push_back(vertexTemp);
//
//
//					//�C���f�b�N�X���������B��Ԍ��Ƀv�b�V���������̂��Q�Ƃ���
//					
//					//�}�b�v�̃L�[�̒l���x�N�^�̃v�b�V�����ꂽ�ʒu�ɂ���
//					//�x�N�^�Y�����i���_�j�}�b�v�L�[�i�C���f�b�N�X�̔ԍ��j�}�b�v�v�fUV���
//					indexRefs[index.indexes[i]][static_cast<int>(vertexes.vertexes.size() - 1)] = std::pair<KLib::Math::Vector3, KLib::Math::Vector2>(normals[index.indexes[i]], uvSet[uvIndexList[i]]);
//					index.indexes[i] = static_cast<int>(vertexes.vertexes.size() - 1); 
//					break;
//				}
//			}
//		}
//	}
//
//}

void FbxLoader::FixVertex(Fbx::FbxModel* model, Fbx::VertexResource& resource)
{
	//���_�Ɠ��������m�ۂ��ꂽ�x�N�^�z��B���̒��_������Q�Ƃ��ꂽ������������B
	std::vector<std::map<int, std::pair<KLib::Math::Vector3, KLib::Math::Vector2>>> indexRefs(resource.pos.size());
	//���_�C���f�b�N�X�������񂷁B���̃X�e�b�v�ł͓������_������Q�Ƃ��ꂽ�����m�F����B�����Q��ȏ�Q�Ƃ����Ȃ�A��̃X�e�b�v�Łu���ȏ�Q�Ƃ���ĂāA�ŏ��ɎQ�Ƃ��ꂽUV�ƈႤUV�l���g���v���_��T��
	model->vertexesInfo.vertexes.resize(resource.pos.size());
	for (int i = 0; i < resource.indexes.size(); i++) {


		if (indexRefs[resource.indexes[i]].size() == 0) {

			indexRefs[resource.indexes[i]][i] = std::pair<KLib::Math::Vector3, KLib::Math::Vector2>(resource.normal[resource.indexes[i]], resource.texCoord[resource.uvIndexList[i]]);
			//����ɎQ�Ƃ���钸�_�͂������ꂿ�Ⴆ
			model->vertexesInfo.vertexes[resource.indexes[i]].pos = resource.pos[resource.indexes[i]];
			model->vertexesInfo.vertexes[resource.indexes[i]].texCoord = resource.texCoord[resource.uvIndexList[i]];
			model->vertexesInfo.vertexes[resource.indexes[i]].normal = resource.normal[i];
			continue;
		}

		for (auto itr = indexRefs[resource.indexes[i]].begin(); itr != indexRefs[resource.indexes[i]].end(); itr++) {
			//����ɑ�������l�Ȃ�Ώ������X�L�b�v�B���̗v�f�ֈړ�
			if (model->vertexesInfo.vertexes[resource.indexes[i]].texCoord == resource.texCoord[resource.uvIndexList[i]] && model->vertexesInfo.vertexes[resource.indexes[i]].normal == resource.normal[i]) {
				continue;

			}
			else if (itr->second.second != resource.texCoord[resource.uvIndexList[i]] || itr->second.first != resource.normal[i]) {//���������ꂪ�V�����Q�Ə��i�@����UV���Ⴄ�j�Ȃ�Β��_�����R�s�[

				Fbx::FbxVertex vertexTemp = model->vertexesInfo.vertexes[resource.indexes[i]];

				//���_���̃P�c�ɉ������ށBUV�l����������
				vertexTemp.pos = resource.pos[resource.indexes[i]];
				vertexTemp.texCoord = resource.texCoord[resource.uvIndexList[i]];
				vertexTemp.normal = resource.normal[i];
				model->vertexesInfo.vertexes.push_back(vertexTemp);

				//�C���f�b�N�X���������B��Ԍ��Ƀv�b�V���������̂��Q�Ƃ���

				//�}�b�v�̃L�[�̒l���x�N�^�̃v�b�V�����ꂽ�ʒu�ɂ���
				//�x�N�^�Y�����i���_�j�}�b�v�L�[�i�C���f�b�N�X�̔ԍ��j�}�b�v�v�fUV���
				indexRefs[resource.indexes[i]][static_cast<int>(model->vertexesInfo.vertexes.size() - 1)] = std::pair<KLib::Math::Vector3, KLib::Math::Vector2>(resource.normal[resource.indexes[i]], resource.texCoord[resource.uvIndexList[i]]);
				resource.indexes[i] = static_cast<int>(model->vertexesInfo.vertexes.size() - 1);
				break;
			}
			else if (itr->second.second == resource.texCoord[resource.uvIndexList[i]] && itr->second.first == resource.normal[i]) {//��������UV�l&&�����@�������v�f�y�A���������Ȃ�

				//�C���f�b�N�X��������
				//�ŏ��ɂ���l�̃L�[�l�͂��̒��_�̈ʒu�ɏ��������Ă���i���������C���f�b�N�X���ɂȂ��l�ɂ��Ă���̂ő�����邱�Ƃ͂Ȃ��ƐM�������j
				resource.indexes[i] = itr->first;

			}
			
		}
	}
	model->indexes.indexCount = static_cast<int>(resource.indexes.size());
	model->indexes.indexes = std::move(resource.indexes);
}

Fbx::FbxModel * FbxLoader::MainLoad(fbxsdk::FbxMesh* mesh,std::string path)
{
	Fbx::FbxModel* model= new Fbx::FbxModel();
	
	////�C���f�b�N�X�̃��[�h
	//model->indexes = std::move( IndexLoad(mesh));
	////���_���W�̎擾
	//model->vertexesInfo = std::move(VertexLoad(mesh));
	////�@���z��̎擾
	//std::vector<KLib::Math::Vector3>normals = std::move(NormalLoad(mesh,model->indexes));
	////UV�z��̎擾
	//std::vector<int >uvIndexList;
	//std::vector<KLib::Math::Vector2>texCoord = std::move(UVLoad(mesh, uvIndexList, model->indexes));

	//Fbx::VertexResource resource;

	//�C���f�b�N�X�̃��[�h
	//IndexLoad(mesh, resource);
	////���_���W�̎擾
	//VertexLoad(mesh, resource);

	TmpVertexInfo(mesh);

	FixVertexInfo(model, mesh);

	////�@���z��̎擾
	//NormalLoad(mesh, resource);
	////UV�z��̎擾
	//UVLoad(mesh, resource);

	////���_����
	//FixVertex(model, resource);
	////�}�e���A���̃��[�h
	//model->materials = std::move(MaterialLoad(mesh));
	////�e�N�X�`���̃��[�h
	//model->textures = std::move(TextureLoad(mesh, path));

	//�{�[�����
	//std::vector<Fbx::FbxBoneInfo> bones = std::move(SkeltonLoad(mesh, path));

	return model;
}

