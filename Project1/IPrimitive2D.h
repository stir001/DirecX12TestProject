#pragma once
#include <DirectXMath.h>
#include <vector>

class VertexBufferObject;
struct D3D12_VERTEX_BUFFER_VIEW;


struct Primitive2DVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};


class IPrimitive2D
{
public:
	IPrimitive2D(int vertexCount);
	virtual ~IPrimitive2D();

	virtual void Draw() const = 0;
protected:
	std::vector<Primitive2DVertex> mVertices;
	VertexBufferObject* mVertexBuffer;
};

