#pragma once
#include "Model.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <DirectXMath.h>

class FbxModelDataConverter;

namespace Fbx {

	struct FbxIndexes{
		std::vector<int>			indexes;
		int							indexCount;
	};
	
	struct UVSet {
		DirectX::XMFLOAT2 texCoord;
		std::string			uvSetName;
	};

	struct FbxVertexElement
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texCoord;
	};

	struct FbxVertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texCoord;
		std::vector<int>	boneIndex;
		std::vector<float>	boneWeight;
	};

	struct FbxVertexesInfo {
		std::vector<FbxVertex> vertexes;
	};

	struct FbxMaterial {
		DirectX::XMFLOAT3	ambient;
		float				ambientFactor;
		DirectX::XMFLOAT3	diffuse;
		float				diffuseFactor;
		DirectX::XMFLOAT3	emissive;
		float				emissiveFactor;
		DirectX::XMFLOAT3	bump;
		float				bumpFactor;
		DirectX::XMFLOAT3	transparent;
		float				transparentFactor;
		DirectX::XMFLOAT3	specular;
		float				specularFactor;
		float				shininess;
		DirectX::XMFLOAT3	reflection;
		float				reflectionPower;

		unsigned int		surfaceVertex;
	};


	struct FbxTexture
	{
		std::string		textureName;
		std::string		texturePath;
		std::string		uvSetName;
		unsigned int	textureCount;
	};

	struct FbxTextureInfo {
		std::vector<Fbx::FbxTexture> textures;
	};

	struct FbxBoneInfo {
		DirectX::XMMATRIX	initMatrix;
		int					index;
		std::string			boneName;
		std::vector<float>	affectedVertexWeight;
		std::vector<int>	affectedVertexIndex;
	};

	struct FbxBone {
		DirectX::XMMATRIX	initMatrix;
		int					index;
		std::string			boneName;
	};

	struct FbxModelData
	{
		std::string modelPath;
		FbxIndexes					indexes;
		FbxVertexesInfo				vertexesInfo;
		std::vector<FbxMaterial>	materials;
		std::vector<FbxTextureInfo>	textures;
		std::vector<FbxBone>		bones;
	};

	class FbxModel : public Model
	{
		friend FbxModelDataConverter;
	public:
		FbxModel();
		~FbxModel();

		std::vector<int> mIndexes;
		std::vector<FbxVertex> mVertexes;
		std::vector<FbxVertexElement> mVertexElements;
		std::vector<Fbx::FbxTexture> mTextures;
		std::vector<FbxBone> mBones;

		void SetTexture();
	private:
	};
}

