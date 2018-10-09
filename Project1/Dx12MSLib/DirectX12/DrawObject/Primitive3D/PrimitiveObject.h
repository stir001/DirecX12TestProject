#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <wrl.h>

struct ID3D12GraphicsCommandList;
class VertexBufferObject;
class PipelineStateObject;
class RootSignatureObject;
class Dx12CommnadList;

const DirectX::XMFLOAT4 INIT_COLOR = { 0.0f,0.5f,0.5f,1.0f };

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
		color = INIT_COLOR;
		uv = { 0, 0 };
	}

	PrimitiveVertex(DirectX::XMFLOAT3& inpos, DirectX::XMFLOAT3& innorm, DirectX::XMFLOAT2& inuv)
	{
		pos = { inpos.x, inpos.y, inpos.z, 1.0f };
		normal = { innorm.x, innorm.y, innorm.z, 1.0f };
		uv = inuv;
		color = INIT_COLOR;
	}

	PrimitiveVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		pos = { x, y, z, 1.0f };
		normal = { nx, ny, nz, 1.0f };
		uv = { u,v };
		color = INIT_COLOR;
	}
};

class PrimitiveObject
{
public:
	PrimitiveObject(const std::string& name);
	~PrimitiveObject();

	virtual const std::string& GetName() const;
	virtual const std::vector<PrimitiveVertex>& GetVertices() const;
	virtual const std::vector<unsigned int>& GetIndices() const;
protected:
	std::vector<PrimitiveVertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::string mName;
};
