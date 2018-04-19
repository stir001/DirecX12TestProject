#pragma once
#include <KLib.h>
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Fbx {

	struct FbxIndexes{
		std::vector<int>			indexes;
		int							indexCount;
	};
	
	struct UVSet {
		KLib::Math::Vector2 texCoord;
		std::string			uvSetName;
	};


	struct FbxVertex {
		KLib::Math::Vector3 pos;
		KLib::Math::Vector3 normal;
		KLib::Math::Vector2 texCoord;
		std::vector<int>	boneIndex;
		std::vector<float>	boneWeight;
	};

	struct FbxVertexesInfo {
		std::vector<FbxVertex> vertexes;
	};

	struct FbxMaterial {
		KLib::Math::Vector3	ambient;
		float				ambientFactor;
		KLib::Math::Vector3	diffuse;
		float				diffuseFactor;
		KLib::Math::Vector3	emissive;
		float				emissiveFactor;
		KLib::Math::Vector3	bump;
		float				bumpFactor;
		KLib::Math::Vector3	transparent;
		float				transparentFactor;
		KLib::Math::Vector3	specular;
		float				specularFactor;
		float				shininess;
		KLib::Math::Vector3	reflection;
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
		KLib::Math::Matrix	initMatrix;
		int					index;
		std::string			boneName;
		std::vector<float>	affectedVertexWeight;
		std::vector<int>	affectedVertexIndex;
	};

	struct FbxBone {
		KLib::Math::Matrix	initMatrix;
		int					index;
		std::string			boneName;
	};

	struct FbxModel
	{
		FbxIndexes					indexes;
		FbxVertexesInfo				vertexesInfo;
		std::vector<FbxMaterial>	materials;
		std::vector<FbxTextureInfo>	textures;
		std::vector<FbxBone>		bones;
	};
}

