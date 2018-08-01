#pragma once
#include "DrawController3D.h"
#include <vector>

class PrimitiveObject;
class VertexBufferObject;
class ConstantBufferObject;
class DescriptorHeapObject;

class PrimitiveController :
	public DrawController3D
{
public:
	PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
		,Microsoft::WRL::ComPtr<ID3D12Device>& dev
		, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	~PrimitiveController();

	void Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions);

protected:
	std::shared_ptr<PrimitiveObject> mPrimitive;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::vector<DirectX::XMFLOAT4X4> mInstancesMatrix;
	std::shared_ptr<ConstantBufferObject> mInstanceMatrixBuffer;
	std::shared_ptr<DescriptorHeapObject> mDescHeap;
};

