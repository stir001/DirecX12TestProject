#pragma once
#include "DrawObject/Model.h"
#include "FbxStructure.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <DirectXMath.h>

class FbxModelDataConverter;
struct ID3D12GraphicsCommandList;

namespace Fbx {

	class FbxModel : public Model
	{
		friend FbxModelDataConverter;
	public:
		enum eROOT_PARAMATER_INDEX
		{
			eROOT_PARAMATER_INDEX_DIFFUSE,
			eROOT_PARAMATER_INDEX_SPECULAR,
			eROOT_PARAMATER_INDEX_BUMP,
			eROOT_PARAMATER_INDEX_CAMERA,
			eROOT_PARAMATER_INDEX_LIGHT,
			eROOT_PARAMATER_INDEX_MATRIX,
			eROOT_PARAMATER_INDEX_MAX,
		};

		FbxModel();
		~FbxModel();

		std::vector<std::shared_ptr<TextureObject>>& GetTextureObjects();

		std::vector<unsigned int> mIndexes;
		std::vector<FbxVertex> mVertexes;
		std::vector<Fbx::FbxBone> mBones;
	private:
	};
}

