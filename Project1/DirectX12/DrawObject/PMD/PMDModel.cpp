#include "PMDModel.h"
#include "Dx12Ctrl.h"
#include "ConstantBufferObject.h"
#include "RootSignatureObject.h"
#include "TextureObject.h"
#include "VMDPlayer.h"

#include <algorithm>

PMDModel::PMDModel()
{
}

PMDModel::~PMDModel()
{
	Model::~Model();

}

void PMDModel::SetMaterialBuffer()
{
	
}

std::shared_ptr<PMDModel> PMDModel::Create()
{
	class CreateHelperPMD : public PMDModel
	{
	public:
		CreateHelperPMD() : PMDModel() {}
		~CreateHelperPMD(){}
	};

	auto helper = std::make_shared<CreateHelperPMD>();
	return std::move(helper);
}

