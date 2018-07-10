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
	void SetPositon(DirectX::XMFLOAT3& pos);
	void SetScale(float scale);
	void AddRotaX(float deg);
	void AddRotaY(float deg);
	void AddRotaZ(float deg);
	void SetRotaQuaternion(DirectX::XMFLOAT4& quaternion);
private:
	ConstantBufferObject * mModelMatrixBuffer;
	DirectX::XMMATRIX mModelMatrix;
	float mScale;
	std::shared_ptr<Fbx::FbxModel> mModel;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMMATRIX mRotationMatrix;
	DirectX::XMFLOAT4 mQuaternion;
	ConstantBufferObject* mBoneMatrixBuffer;
	std::vector<DirectX::XMMATRIX> mBoneMatrix;
	std::shared_ptr<DirectionalLight> mDirLight;

	void UpdateMatrix();
	void SetModelMatrix();
};

