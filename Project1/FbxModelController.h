#pragma once
#include "ModelController.h"
#include <vector>
#include <memory>
#include <DirectXMath.h>

class ConstantBufferObject;
class DirectionalLight;
class FbxModelDataConverter;


namespace Fbx
{
	class FbxModel;
}

class FbxModelController :
	public ModelController
{
	friend FbxModelDataConverter;
public:
	FbxModelController();
	FbxModelController(std::shared_ptr<Fbx::FbxModel> model);
	~FbxModelController();

	void Draw();
	void SetLight(std::shared_ptr<DirectionalLight>);
private:
	std::shared_ptr<Fbx::FbxModel> mModel;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mRotation;
	ConstantBufferObject* mBoneMatrixBuffer;
	std::vector<DirectX::XMMATRIX> mBoneMatrix;
	std::shared_ptr<DirectionalLight> mDirLight;
};

