#pragma once
#include "FbxStructure.h"
#include <d3d12.h>
#include <wrl.h>

namespace Fbx
{
	struct FbxModelData;
	class FbxModel;
}

class FbxModelController;

class FbxModelDataConverter
{
public:
	FbxModelDataConverter();
	~FbxModelDataConverter();

	Fbx::FbxModel* ConvertToFbxModel(const Fbx::FbxModelData* data);

private:
	Fbx::FbxModel* mModel;
	std::string mRelativePath;
	const Fbx::FbxModelData* mConvertData;

	void ConvertIndex(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void ConvertVertex(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void ConvertTexture(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void ConvertBone(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
};

