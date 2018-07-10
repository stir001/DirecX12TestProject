#pragma once
#include "DrawObjectLoader.h"
#include "FbxStructure.h"

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
	class FbxModel;
}

struct NodeTree
{
	std::string nodeName;
	fbxsdk::FbxAMatrix globalPosition;
	fbxsdk::FbxAMatrix globalOffsetPosition;
	std::vector<NodeTree> children;
};


class FbxLoader : public DrawObjectLoader
{
public:
	~FbxLoader();

	static void Create() {
		if (mInstance == nullptr)
			mInstance.reset(new FbxLoader());
	};
	static void Destroy() {
		if (mInstance != nullptr) {
			mInstance.reset();
			mInstance = nullptr;
		}
	};
	static std::unique_ptr<FbxLoader>& Instance() {
		return mInstance;
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

	//variables
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
	
	FbxModelDataConverter* mModelConverter;
	FbxMotionConverter* mMotionConverter;

	static std::unique_ptr<FbxLoader> mInstance;
	std::vector<fbxsdk::FbxMesh*> mMeshDatas;
	std::vector<fbxsdk::FbxNode*> mNodeDatas;
	std::vector<Fbx::AnimationData> mAnimCurves;
	std::vector<Fbx::SkeletonAnimationData> mSkeletonMatrix;
	fbxsdk::FbxAMatrix mGeometryOffset;

	std::vector<Fbx::TmpVertex> mTmpVertices;
	std::vector<int> mTmpIndexes;
	std::vector<Fbx::TmpBone> mBones;

	bool LoaderInitializie(std::string fbxPath);
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	Fbx::FbxModelData* ConnectMeshes(std::vector<Fbx::FbxModelData*>& datas);

	//í∏ì_êÆçáópä÷êî
	int CheckVertexDiff(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex);

	void FixVertexInfo(Fbx::FbxModelData* model, fbxsdk::FbxMesh* mesh);

	void StackSearchNode(fbxsdk::FbxNode* parent, FbxNodeAttribute::EType searchtype, NodeTree& parentodetree);

	void StackNode(fbxsdk::FbxNode* pNode, FbxNodeAttribute::EType type, fbxsdk::FbxArray<fbxsdk::FbxNode*>& nodeArray);

	Fbx::FbxModelData* MainLoad(fbxsdk::FbxMesh* mesh, std::string path);

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

	fbxsdk::FbxAMatrix GetGlobalPosition(fbxsdk::FbxNode* pNode, const fbxsdk::FbxTime& pTime, fbxsdk::FbxPose* pPose = nullptr, fbxsdk::FbxAMatrix* pParentGlobalPosition = nullptr);

	fbxsdk::FbxAMatrix GetPoseMatrix(fbxsdk::FbxPose* pPose, int pNodeIndex);

	fbxsdk::FbxAMatrix GetGeometry(fbxsdk::FbxNode* pNode);

	void DrawNodeRecursive(fbxsdk::FbxNode* pNode, fbxsdk::FbxTime& pTime, fbxsdk::FbxAnimLayer* pAnimLayer,
		fbxsdk::FbxAMatrix& pParentGlobalPosition, fbxsdk::FbxPose* pPose);

	void DrawNode(fbxsdk::FbxNode* pNode,
		fbxsdk::FbxTime& pTime,
		fbxsdk::FbxAnimLayer* pAnimLayer,
		fbxsdk::FbxAMatrix& pParentGlobalPosition,
		fbxsdk::FbxAMatrix& pGlobalPosition,
		fbxsdk::FbxPose* pPose);

	void ComputeClusterDeformation(const fbxsdk::FbxAMatrix& pGlobalPosition,
		fbxsdk::FbxMesh* pMesh,
		fbxsdk::FbxCluster* pCluster,
		fbxsdk::FbxAMatrix& pVertexTransformMatrix,
		fbxsdk::FbxTime pTime,
		fbxsdk::FbxPose* pPose);

	void ComputeLinearDeformation(fbxsdk::FbxAMatrix& pGlobalPosition,
		fbxsdk::FbxMesh* pMesh,
		fbxsdk::FbxTime& pTime,
		fbxsdk::FbxVector4* pVertexArray,
		fbxsdk::FbxPose* pPose);

	std::vector<fbxsdk::FbxTime> ExtractingKeyFrames(fbxsdk::FbxScene* scene, unsigned int meshId, std::vector<fbxsdk::FbxNode*>& linkNode);

	const NodeTree* GetNodeTree(std::string nodename);

	const NodeTree* GetNodeTreeRecursive(std::string nodeName, const NodeTree& nodeTree);

};

