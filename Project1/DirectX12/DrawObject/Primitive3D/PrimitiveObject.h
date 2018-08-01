#pragma once
#include "DrawController3D.h"

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <wrl.h>

struct ID3D12GraphicsCommandList;
class VertexBufferObject;
class PipelineStateObject;
class RootSignatureObject;
class Dx12CommnadList;

struct PrimitiveVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
	PrimitiveVertex()
	{
		pos = { 0, 0, 0, 1};
		normal = { 0, 0, 0, 1 };
		color = { 1, 1, 1, 1 };
		uv = { 0, 0 };
	}

	PrimitiveVertex(DirectX::XMFLOAT3& inpos, DirectX::XMFLOAT3& innorm, DirectX::XMFLOAT2& inuv)
	{
		pos = { inpos.x, inpos.y, inpos.z, 1.0f };
		normal = { innorm.x, innorm.y, innorm.z, 1.0f };
		uv = inuv;
		color = { 1, 1, 1, 1 };
	}

	PrimitiveVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		pos = { x, y, z, 1.0f };
		normal = { nx, ny, nz, 1.0f };
		uv = { u,v };
		color = { 1, 1, 1, 1 };
	}
};

class PrimitiveObject
{
public:
	PrimitiveObject(const std::string& name);
	~PrimitiveObject();

	virtual const std::string& GetName() const;
	virtual std::vector<PrimitiveVertex>& GetVertices();
protected:
	std::vector<PrimitiveVertex> mVertices;
	std::string mName;
};

