#include "FbxLoader.h"
#include "FbxModel.h"
#include <fbxsdk.h>
#include <memory>
#include <exception>
#include <cassert>
#include <algorithm>
#include <Windows.h>


FbxLoader* FbxLoader::instance = nullptr;

Fbx::FbxModelData* FbxLoader::LoadMesh(std::string modelPath)
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
	
	std::vector<Fbx::FbxModelData*> models(m_meshDatas.size());

	for (int i = 0; i < m_meshDatas.size(); i++)
	{
		models[i] = MainLoad(m_meshDatas[i], modelPath);
		models[i]->modelPath = modelPath;
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
		}
	}
}

Fbx::FbxModelData * FbxLoader::LoadAnimation(std::string modelPath)
{
	return nullptr;
}

//�e�X�g�p���[�J���֐�
void StoreFbxMatrixToKLibMatrix(const fbxsdk::FbxAMatrix& fmat, DirectX::XMMATRIX& kmat)
{
	kmat.r[0].m128_f32[0] = static_cast<float>(fmat.Get(0, 0)); kmat.r[0].m128_f32[1] = static_cast<float>(fmat.Get(0, 1)); kmat.r[0].m128_f32[2] = static_cast<float>(fmat.Get(0, 2)); kmat.r[0].m128_f32[3] = static_cast<float>(fmat.Get(0, 3));
	kmat.r[1].m128_f32[0] = static_cast<float>(fmat.Get(1, 0)); kmat.r[1].m128_f32[1] = static_cast<float>(fmat.Get(1, 1)); kmat.r[1].m128_f32[2] = static_cast<float>(fmat.Get(1, 2)); kmat.r[1].m128_f32[3] = static_cast<float>(fmat.Get(1, 3));
	kmat.r[2].m128_f32[0] = static_cast<float>(fmat.Get(2, 0)); kmat.r[2].m128_f32[1] = static_cast<float>(fmat.Get(2, 1)); kmat.r[2].m128_f32[2] = static_cast<float>(fmat.Get(2, 2)); kmat.r[2].m128_f32[3] = static_cast<float>(fmat.Get(2, 3));
	kmat.r[3].m128_f32[0] = static_cast<float>(fmat.Get(3, 0)); kmat.r[3].m128_f32[1] = static_cast<float>(fmat.Get(3, 1)); kmat.r[3].m128_f32[2] = static_cast<float>(fmat.Get(3, 2)); kmat.r[3].m128_f32[3] = static_cast<float>(fmat.Get(3, 3));
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

		DirectX::XMFLOAT2 t_uv;

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
	
	auto beginitr = model->vertexesInfo.vertexes.begin();
	auto enditr = model->vertexesInfo.vertexes.end();
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

	//EBD store texture path
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

Fbx::FbxModelData* FbxLoader::MainLoad(fbxsdk::FbxMesh* mesh, std::string path)
{
	Fbx::FbxModelData* model= new Fbx::FbxModelData();

	TmpVertexInfo(mesh);

	FixVertexInfo(model, mesh);

	return model;
}

