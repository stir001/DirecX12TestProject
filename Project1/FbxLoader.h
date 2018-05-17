#pragma once

#include <fbxsdk.h>
#include <string>
#include <vector>
#include <DirectXMath.h>

namespace Fbx {
	struct FbxModelData;
	struct FbxIndexes;
	struct FbxVertex;
	struct VertexResource;
	struct FbxVertexesInfo;
	struct FbxMaterial;
	struct FbxTexture;
	struct FbxBoneInfo;
	struct FbxTextureInfo;
}

namespace Fbx {
	//���_�����p�̃f�[�^
	struct VertexResource {
		std::vector<DirectX::XMFLOAT3>	pos;
		std::vector<DirectX::XMFLOAT3>	normal;
		std::vector<DirectX::XMFLOAT2>	texCoord;
		std::vector<int>	indexes;
		std::vector<int>	uvIndexList;
		std::vector<int>	normalIndexList;
	};

	struct TmpWeight
	{
		float weight;
		int boneNo;
	};

	struct TmpNormalUV
	{
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		int vertexNo;			//���z�I�Ȓ��_�ԍ�
		bool operator ==(const TmpNormalUV& rval)
		{
			return (this->normal.x == rval.normal.x &&
				this->normal.y == rval.normal.y &&
				this->normal.z == rval.normal.z &&
				this->uv.x == rval.uv.x &&
				this->uv.y == rval.uv.y);
		}
	};

	struct TmpVertex
	{
		DirectX::XMFLOAT3 pos;
		std::vector<TmpNormalUV> normalandUV;
		std::vector<TmpWeight> weights;
		int refcount;
	};

	struct TmpBone
	{
		std::string boneName;
		DirectX::XMMATRIX matrix;
	};
}
class FbxLoader
{

	//variables
private:

	fbxsdk::FbxManager* manager;
	static FbxLoader* instance;
	Fbx::VertexResource resource;
	std::vector<fbxsdk::FbxMesh*> m_meshDatas;
	std::vector<Fbx::TmpVertex> m_tmpVertices;
	std::vector<int>	m_tmpIndexes;
	std::vector<Fbx::TmpBone> m_bones;
public:

	//function
private:
	//@param scene �C���|�[�g�ς݃V�[��
	//note:�e�X�g�p�֐��@�������͕Ԃ�l�𑊓�������̂ɕς���K�v����
	void LoadAnimation(fbxsdk::FbxScene* scene);

	//���_�����p�֐�
	int CheckVertexDiff(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex);

	void								TmpVertexInfo(fbxsdk::FbxMesh* mesh);
	void								FixVertexInfo(Fbx::FbxModelData* model, fbxsdk::FbxMesh* mesh);

	void								StackMeshNode(fbxsdk::FbxNode* parent);

	Fbx::FbxModelData*					MainLoad(fbxsdk::FbxMesh* mesh, std::string path);
	FbxLoader() {};
	FbxLoader(const FbxLoader& ref) {};
	FbxLoader(FbxLoader&& ref) {};
	FbxLoader& operator = (const FbxLoader&) {};
	FbxLoader& operator = (FbxLoader&&) {};
public:	
	static void Create() {
		if (instance == nullptr)
			instance = new FbxLoader();
	};
	static void Destroy() {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	};
	static FbxLoader& Instance() {
		return *instance;
	};
	Fbx::FbxModelData*						LoadMesh(std::string modelPath);

	//�܂�������
	Fbx::FbxModelData*						LoadAnimation(std::string modelPath);
	~FbxLoader() = default;
};

