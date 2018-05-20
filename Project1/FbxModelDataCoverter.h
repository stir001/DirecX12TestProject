#pragma once
#include "ModelLoader.h"

namespace Fbx
{
	struct FbxModelData;
	class FbxModel;
}

class FbxModelController;

class FbxModelDataConverter : public ModelLoader
{
public:
	FbxModelDataConverter();
	~FbxModelDataConverter();

	Fbx::FbxModel* ConvertToFbxModel(const Fbx::FbxModelData* data);

private:
	FbxModelController* mCtrl;
	Fbx::FbxModel* mModel;
	const Fbx::FbxModelData* mConvertData;

	void ConvertIndex();
	void ConvertVertex();
	void ConvertTexture();
};

