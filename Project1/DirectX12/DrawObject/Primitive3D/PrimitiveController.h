#pragma once
#include "DrawController3D.h"
#include <vector>

class PrimitiveObject;
class VertexBufferObject;
class IndexBufferObject;
class ConstantBufferObject;
class Dx12DescriptorHeapObject;

class PrimitiveController :
	public DrawController3D
{
public:
	PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
		,Microsoft::WRL::ComPtr<ID3D12Device>& dev
		, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	~PrimitiveController();

	void Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions);
	void Draw();

protected:
	std::shared_ptr<PrimitiveObject> mPrimitive;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<IndexBufferObject> mIndexBuffer;
	std::vector<DirectX::XMFLOAT4X4> mInstancesMatrix;
	std::shared_ptr<VertexBufferObject> mInstanceMatrixBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	void (PrimitiveController::*mUpdate)();
	
	void UpdateBuffer();
	void NonUpdateBuffer();
};

