#pragma once
#include "DrawObjectController.h"
#include "PMDDataStructure.h"

class PMDModel;
class ConstantBufferObject;
class VMDMotion;
class VMDPlayer;
class DirectionalLight;
class Dx12DescriptorHeapObject;
class PipelineStateObject;
class RootSignatureObject;

class PMDController
	: public DrawObjectController
{
	friend class PMDLoader;
public:
	PMDController(std::shared_ptr<PMDModel>& model, std::shared_ptr<DirectionalLight>& dlight, const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	~PMDController();

	void Draw();
	void SetMotion(std::shared_ptr<VMDMotion> motion);
	void PlayMotion(bool loopFlag = false);
	void StopMotion();
	void SetPosition(DirectX::XMFLOAT3& p);
	void SetRota(DirectX::XMFLOAT3& rota);

	void SetLight(std::shared_ptr<DirectionalLight> dlight);
	void SetSubPipeLineState(std::shared_ptr<PipelineStateObject>& pipelineState);
	void SetSubRootsignature(std::shared_ptr<RootSignatureObject>& rootsiganture);
private:
	std::shared_ptr<PMDModel> mModel;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mRotation;
	std::shared_ptr<ConstantBufferObject> mBoneMatrixBuffer;
	std::vector<DirectX::XMMATRIX> mBoneMatrix;
	std::shared_ptr<DirectionalLight> mDirLight;
	std::shared_ptr<VMDPlayer> mVmdPlayer;
	std::shared_ptr<PipelineStateObject> mSubPipeline;
	std::shared_ptr<RootSignatureObject> mSubRootsignature;
	std::unique_ptr<Dx12DescriptorHeapObject> mDescHeap;

	void DrawWhileSetTable();
	void SetTexture(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, PMDMaterial& material);
	void SetMaterial(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount);
	void CreateDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string& name);
	void SetConstantBuffers(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
};

