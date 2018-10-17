#pragma once
#include "DrawObject/DrawController3D.h"
#include "FbxStructure.h"

#include <vector>
#include <memory>
#include <DirectXMath.h>

class ConstantBufferObject;
class VertexBufferObject;
class LightObject;
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
	void SetLight(std::shared_ptr<LightObject> dirlight);

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
	std::shared_ptr<FbxMotionPlayer> mMotionPlayer;
	DirectX::XMFLOAT3 mColor;


	std::vector<Fbx::FbxVertexElement> mVertexElements;
	std::shared_ptr<VertexBufferObject> mCtrlVertexBuffer;
	std::shared_ptr<Fbx::FbxModel> mModel;

	std::shared_ptr<ConstantBufferObject> mDirLightBuffer;
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;

	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::vector<std::shared_ptr<ConstantBufferObject>> mAddConstantBuffers;

	void (FbxModelController::*mBundleUpdate)();

	void UpdateVertex();
	void UpdateBundle();
	void NonBundleUpdate();
	void UpdateDescriptorHeap();
};
