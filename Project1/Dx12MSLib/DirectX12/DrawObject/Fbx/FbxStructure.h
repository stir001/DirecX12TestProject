#pragma once
#include <vector>
#include <DirectXMath.h>
#include <string>
#include <fbxsdk.h>

namespace Fbx
{
	struct FbxIndexes {
		std::vector<unsigned int> indexes;
		int	indexCount;
	};

	struct UVSet {
		DirectX::XMFLOAT2 texCoord;
		std::string	uvSetName;
	};

	struct FbxVertexElement
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT4X4 vertexMatrix;
	};

	struct FbxVertex {
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 texCoord;
		std::vector<int> boneIndex;
		std::vector<float> boneWeight;
		std::vector<std::string> boneName;
	};

	struct FbxVertexesInfo {
		std::vector<FbxVertex> vertexes;
	};

	struct FbxMaterial {
		DirectX::XMFLOAT3 ambient;
		float ambientFactor;

		DirectX::XMFLOAT3 diffuse;
		float diffuseFactor;

		DirectX::XMFLOAT3 emissive;
		float emissiveFactor;

		DirectX::XMFLOAT3 bump;
		float bumpFactor;

		DirectX::XMFLOAT3 transparent;
		float transparentFactor;

		DirectX::XMFLOAT3 specular;
		float specularFactor;

		float shininess;

		DirectX::XMFLOAT3 reflection;
		float reflectionPower;

		unsigned int surfaceVertex;
	};


	struct FbxTexture
	{
		std::string	textureName;
		std::string	texturePath;
		std::string	uvSetName;
		unsigned int textureCount;
	};

	struct FbxTextureInfo {
		std::vector<FbxTexture> textures;
	};

	struct FbxBoneInfo {
		DirectX::XMMATRIX initMatrix;
		int	index;
		std::string	boneName;
		std::vector<float>	affectedVertexWeight;
		std::vector<int> affectedVertexIndex;
	};

	struct FbxBone {
		std::string	boneName;
		int	index;
		DirectX::XMFLOAT4X4 initMatrix;
	};

	struct FbxModelData
	{
		std::string modelPath;
		FbxIndexes	indexes;
		FbxVertexesInfo	vertexesInfo;
		std::vector<FbxMaterial> materials;
		std::vector<FbxTextureInfo>	textures;
		std::vector<FbxBone> bones;
	};

	struct BoneFrameData
	{
		int frame;
		DirectX::XMMATRIX matrix;
	};

	struct BoneMatrixData
	{
		std::string boneName;
		unsigned int currentDataIndex;
		std::vector<BoneFrameData> frameData;
	};

	typedef std::vector<BoneMatrixData> AnimationDatas_t;
}

namespace Fbx {
	//頂点整合用のデータ
	struct VertexResource {
		std::vector<DirectX::XMFLOAT3> pos;
		std::vector<DirectX::XMFLOAT3> normal;
		std::vector<DirectX::XMFLOAT2> texCoord;
		std::vector<int> indexes;
		std::vector<int> uvIndexList;
		std::vector<int> normalIndexList;
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
		int vertexNo;			//仮想的な頂点番号
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
		std::vector<std::string> boneName;
		int refcount;
	};

	struct TmpBone
	{
		std::string boneName;
		DirectX::XMMATRIX matrix;
	};

	struct AnimKeyData
	{
		float value;
		long long int time;
	};

	struct AnimCurveData
	{
		fbxsdk::FbxAnimCurve* curve;
		fbxsdk::FbxArray<AnimKeyData> keys;
	};

	struct AnimationData
	{
		std::string skeletonName;
		AnimCurveData translationX;
		AnimCurveData translationY;
		AnimCurveData translationZ;

		AnimCurveData rotationX;
		AnimCurveData rotationY;
		AnimCurveData rotationZ;

		AnimCurveData scaleX;
		AnimCurveData scaleY;
		AnimCurveData scaleZ;
	};

	struct AnimationMatrix
	{
		fbxsdk::FbxTime frame;
		DirectX::XMMATRIX matrix;
	};

	struct SkeletonAnimationData
	{
		std::string skeletonName;
		std::vector<AnimationMatrix> animMatrix;
	};
}