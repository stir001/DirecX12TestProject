#pragma once
#include <DirectXMath.h>
#include <vector>

struct ID3D12Resource;
struct D3D12_VERTEX_BUFFER_VIEW;
class VertexBufferObject;

struct PrimitiveVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 uv;
	PrimitiveVertex()
	{
		pos = DirectX::XMFLOAT3(0, 0, 0);
		normal = DirectX::XMFLOAT3(0, 0, 0);
		color = DirectX::XMFLOAT3(1, 1, 1);
		uv = DirectX::XMFLOAT2(0, 0);
	}
	PrimitiveVertex(DirectX::XMFLOAT3& p, DirectX::XMFLOAT3& norm, DirectX::XMFLOAT2& coord)
	{
		pos = p;
		normal = norm;
		uv = coord;
		color = DirectX::XMFLOAT3(1, 1, 1);
	}
	PrimitiveVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		pos.x = x;
		pos.y = y;
		pos.z = z;
		normal.x = nx;
		normal.y = ny;
		normal.z = nz;
		uv.x = u;
		uv.y = v;
		color = DirectX::XMFLOAT3(1, 1, 1);
	}
};

class PrimitiveObject
{
public:
	PrimitiveObject();
	~PrimitiveObject();

	virtual void Draw() = 0;
protected:
	std::vector<PrimitiveVertex> mVertices;
	VertexBufferObject* mVertexBuffer;
};

