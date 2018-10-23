#pragma once
#include "DrawObject/DrawObjectLoader.h"
#include "FbxStructure.h"

#include <functional>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <map>
#include <memory>
#include <d3d12.h>

class FbxModelDataConverter;
class FbxModelController;
class FbxMotionData;
class FbxMotionConverter;
class LightObject;

namespace Fbx {
	struct FbxModelData;
	struct FbxIndexes;
	struct FbxVertex;
	struct VertexResource;
	struct FbxVertexesInfo;
	struct FbxMaterial;
	struct FbxTexture;
	struct FbxBoneInfo;
	class FbxModel;
}

struct NodeTree
{
	std::string nodeName;
	fbxsdk::FbxAMatrix globalPosition;
	fbxsdk::FbxAMatrix globalOffsetPosition;
	fbxsdk::FbxNodeAttribute::EType attributeType;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	std::vector<NodeTree> children;
};


class FbxLoader : public DrawObjectLoader
{
public:
	virtual ~FbxLoader();

	static void Destroy() {
		if (mInstance != nullptr) {
			delete mInstance;
			mInstance = nullptr;
		}
	};
	static FbxLoader& Instance() {
		if (mInstance == nullptr)
		{
			mInstance = new FbxLoader();
		}
		return *mInstance;
	};
	std::shared_ptr<FbxModelController>	LoadMesh(const std::string& modelPath);

	void ReleaseModel(const std::string& modelPath);

	void ReleaseNonRefModel();

	void ReleaseAllModel();

	std::shared_ptr<FbxMotionData> LoadAnimation(const std::string& animationPath);

	void ReleaseAnimation(const std::string& animationPath);

	void ReleaseNonRefAnimation();

	void ReleaseAllAnimation();

	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

private:
	FbxLoader();
	FbxLoader(const FbxLoader& ref) {};
	FbxLoader(FbxLoader&& ref) {};
	FbxLoader& operator = (const FbxLoader&) {};
	FbxLoader& operator = (FbxLoader&&) {};

	fbxsdk::FbxManager* mManager;
	fbxsdk::FbxScene* mScene;
	fbxsdk::FbxImporter* mImporter;
	fbxsdk::FbxPose* mPose;

	std::map<std::string, std::shared_ptr<Fbx::FbxModel>> mModelDatas;
	std::map<std::string, std::shared_ptr<FbxMotionData>> mAnimationDatas;
	NodeTree mNodeTree;
	
	std::shared_ptr<FbxModelDataConverter> mModelConverter;
	std::shared_ptr<FbxMotionConverter> mMotionConverter;

	static FbxLoader* mInstance;
	std::vector<fbxsdk::FbxMesh*> mMeshDatas;
	std::vector<fbxsdk::FbxNode*> mNodeDatas;
	std::vector<Fbx::AnimationData> mAnimCurves;
	std::vector<Fbx::SkeletonAnimationData> mSkeletonMatrix;
	fbxsdk::FbxAMatrix mGeometryOffset;

	std::vector<Fbx::TmpVertex> mTmpVertices;
	std::vector<int> mTmpIndexes;
	std::vector<Fbx::TmpBone> mBones;
	std::vector<Fbx::FbxSkeleton> mSkeltons;
	std::vector<unsigned int> mSkeltonIndices;
	std::vector<Fbx::MaterialIndexSet> mMaterialSets;

	std::shared_ptr<LightObject> mLight;

	bool LoaderInitializie(std::string fbxPath);
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	std::shared_ptr<Fbx::FbxModelData> ConnectMeshes(std::vector<std::shared_ptr<Fbx::FbxModelData>>& datas);

	//���_�����p�֐�
	int CheckVertexDiff(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex);

	void FixVertexInfo(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh* mesh);

	void StackSearchNode(fbxsdk::FbxNode* parent, fbxsdk::FbxNodeAttribute::EType searchtype, NodeTree& parenTree, std::function<void(fbxsdk::FbxNode*)> hitFunction);

	void StackNode(fbxsdk::FbxNode* pNode, fbxsdk::FbxNodeAttribute::EType type, fbxsdk::FbxArray<fbxsdk::FbxNode*>& nodeArray);

	std::shared_ptr<Fbx::FbxModelData> MainLoad(fbxsdk::FbxMesh* mesh, std::string path);

	void LoadVertexPosition(fbxsdk::FbxMesh* mesh);

	void LoadVertexNormal(fbxsdk::FbxMesh* mesh);

	void LoadVertexUV(fbxsdk::FbxMesh* mesh);

	void LoadBone(fbxsdk::FbxMesh* mesh);

	void ClearTmpInfo();

	void LoadAnimationMain(fbxsdk::FbxScene* scene, unsigned int meshId);

	void LoadCurve(Fbx::AnimCurveData& curveData);

	void StackAnimationTime(const fbxsdk::FbxArray<Fbx::AnimKeyData>& data, std::vector<fbxsdk::FbxTime>& stack);

	void DestroyScence(fbxsdk::FbxScene* scene);

	void DestroyNode(fbxsdk::FbxNode* node);

	void LoadMatarial(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh* mesh);

	std::vector<fbxsdk::FbxTime> ExtractingKeyFrames(fbxsdk::FbxScene* scene, unsigned int meshId, std::vector<fbxsdk::FbxNode*>& linkNode);

	//const NodeTree* GetNodeTree(std::string nodename);

	//const NodeTree* GetNodeTreeRecursive(std::string nodeName, const NodeTree& nodeTree);
};

