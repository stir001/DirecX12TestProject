#pragma once
#include "DrawController3D.h"
#include "FbxStructure.h"

#include <vector>
#include <memory>
#include <DirectXMath.h>

class ConstantBufferObject;
class VertexBufferObject;
class DirectionalLight;
class FbxModelDataConverter;
class RootSignatureObject;
class PipelineStateObject;
class FbxMotionPlayer;
class FbxMotionData;
class Dx12DescriptorHeapObject;
class Dx12Camera;

namespace Fbx
{
	class FbxModel;
}

class FbxModelController :
	public DrawController3D
{
	friend FbxModelDataConverter;
public:
	FbxModelController(std::shared_ptr<Fbx::FbxModel>& model, 
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);
	~FbxModelController();

	void Draw();
	void SetLight(std::shared_ptr<DirectionalLight>& dirlight);

	/*void SetPositon(const DirectX::XMFLOAT3& pos);
	void SetScale(float scale);
	void AddRotaX(float deg);
	void AddRotaY(float deg);
	void AddRotaZ(float deg);
	void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion);*/

	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);
	void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	void SetMotion(std::shared_ptr<FbxMotionData>& motion, bool isLoop = true);
	void StopMotion() const;
	void RestartMotion() const;
	bool IsMotionEnd() const;
	void AddConstantBuffer(std::shared_ptr<ConstantBufferObject>& buffer);
	std::string GetModelName() const;
	std::string GetModelPath() const;
private:
	FbxMotionPlayer* mMotionPlayer;
	DirectX::XMFLOAT3 mColor;


	std::vector<Fbx::FbxVertexElement> mVertexElements;
	std::shared_ptr<VertexBufferObject> mCtrlVertexBuffer;
	std::shared_ptr<Fbx::FbxModel> mModel;

	//DirectX::XMFLOAT4X4 mModelMatrix;
	//float mScale;
	//DirectX::XMFLOAT3 mPos;
	//DirectX::XMFLOAT4X4 mRotationMatrix;
	//DirectX::XMFLOAT4 mQuaternion;

	std::shared_ptr<ConstantBufferObject> mDirLightBuffer;
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;

	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::vector<std::shared_ptr<ConstantBufferObject>> mAddConstantBuffers;

	void (FbxModelController::*mBundleUpdate)();

	//void UpdateMatrix();
	void UpdateVertex();
	void UpdateBundle();
	void NonBundleUpdate();
	void UpdateDescriptorHeap();
};
