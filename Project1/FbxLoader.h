#pragma once

#include <fbxsdk.h>
#include <string>
#include <vector>
#include <map>
#include <KLib.h>
namespace Fbx {
	struct FbxModel;
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
	//頂点整合用のデータ
	struct VertexResource {
		std::vector<KLib::Math::Vector3>	pos;
		std::vector<KLib::Math::Vector3>	normal;
		std::vector<KLib::Math::Vector2>	texCoord;
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
		KLib::Math::Vector3 normal;
		KLib::Math::Vector2 uv;
		int vertexNo;			//仮想的な頂点番号
		bool operator ==(const TmpNormalUV& rval)
		{
			return (this->normal == rval.normal && this->uv == rval.uv);
		}
	};

	struct TmpVertex
	{
		KLib::Math::Vector3 pos;
		std::vector<TmpNormalUV> normalandUV;
		std::vector<TmpWeight> weights;
		int refcount;
	};

	struct TmpBone
	{
		std::string boneName;
		KLib::Math::Matrix matrix;
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
	//@param scene インポート済みシーン
	//note:テスト用関数　実装時は返り値を相当するものに変える必要あり
	void LoadAnimation(fbxsdk::FbxScene* scene);


	//頂点整合用関数
	int CheckVertexDiff(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex);

	void								TmpVertexInfo(fbxsdk::FbxMesh* mesh);
	void								FixVertexInfo(Fbx::FbxModel* model, fbxsdk::FbxMesh* mesh);

	std::string							GetRelativeTexturePath(std::string modelPath,std::string texturePath);
	Fbx::FbxIndexes						IndexLoad(fbxsdk::FbxMesh* mesh);
	Fbx::FbxVertexesInfo				VertexLoad(fbxsdk::FbxMesh* mesh);
	std::vector<KLib::Math::Vector3>	NormalLoad(fbxsdk::FbxMesh* mesh, Fbx::FbxIndexes& indexes);
	std::vector<KLib::Math::Vector2>	UVLoad(fbxsdk::FbxMesh* mesh , std::vector<int>& uvIndexList, Fbx::FbxIndexes& indexes);

	void								IndexLoad(fbxsdk::FbxMesh* mesh, Fbx::VertexResource& resource);
	void								VertexLoad(fbxsdk::FbxMesh* mesh, Fbx::VertexResource& resource);
	void								NormalLoad(fbxsdk::FbxMesh* mesh, Fbx::VertexResource& resource);
	void								UVLoad(fbxsdk::FbxMesh* mesh, Fbx::VertexResource& resource);

	std::vector<Fbx::FbxMaterial>		MaterialLoad(fbxsdk::FbxMesh* mesh);
	std::vector<Fbx::FbxTextureInfo>	TextureLoad(fbxsdk::FbxMesh* mesh, std::string path);
	std::vector<Fbx::FbxBoneInfo>		SkeltonLoad(fbxsdk::FbxMesh* mesh, std::string path);
	void 								FixSkelton(std::vector<Fbx::FbxBoneInfo>& bones);

	fbxsdk::FbxNode*					SerchNode(fbxsdk::FbxNode* node, fbxsdk::FbxNodeAttribute::EType type);
	void								StackMeshNode(fbxsdk::FbxNode* parent);

	void								FixVertex(Fbx::FbxVertexesInfo& vertexes, Fbx::FbxIndexes& index, std::vector<KLib::Math::Vector3>& normals, std::vector<KLib::Math::Vector2>& uvSet,  std::vector<int>& uvIndexList);
	void								FixVertex(Fbx::FbxModel* model, Fbx::VertexResource& resource);

	Fbx::FbxModel*						MainLoad(fbxsdk::FbxMesh* mesh, std::string path);
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
	Fbx::FbxModel*						LoadMesh(std::string modelPath);

	//まだ未実装
	Fbx::FbxModel*						LoadAnimation(std::string modelPath);
	~FbxLoader() = default;
};

