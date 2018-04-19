#include "PrimitivePlane.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "XMFloat3Operators.h"

using namespace DirectX;

PrimitivePlane::PrimitivePlane()
{
}

PrimitivePlane::PrimitivePlane(DirectX::XMFLOAT3 p, float len, float hei, DirectX::XMFLOAT3 norm)
{
	pos = p;
	length = len;
	height = hei;
	norm = NormalizeXMFloat3(norm);

	DirectX::XMVECTOR leftUp = { -length / 2.0f, 0, height / 2.0f };
	DirectX::XMVECTOR rightUp = { length / 2.0f, 0, height / 2.0f };
	DirectX::XMVECTOR leftDown = { -length / 2.0f, 0, -height / 2.0f };
	DirectX::XMVECTOR rightDown = { length / 2.0f, 0,  -height / 2.0f };

	DirectX::XMVECTOR center = { p.x, p.y, p.z };

	DirectX::XMFLOAT3 defNorm = { 0, 1, 0 };

	DirectX::XMFLOAT3 inNorm = { norm.x, norm.y, norm.z };

	float cos = DotXMFloat3(defNorm, inNorm);
	float rad = acosf(DotXMFloat3(defNorm, inNorm));


	DirectX::XMMATRIX m = XMMatrixIdentity();

	if (defNorm != inNorm)
	{
		DirectX::XMFLOAT3 cross = CrossXMFloat3(defNorm, inNorm);

		DirectX::XMVECTOR q = CreateQuoternion(cross, XMConvertToDegrees(rad));

		m = XMMatrixRotationQuaternion(q);
	}

	leftUp = XMVector3Transform(leftUp, m);
	rightUp = XMVector3Transform(rightUp, m);
	leftDown = XMVector3Transform(leftDown, m);
	rightDown = XMVector3Transform(rightDown, m);

	DirectX::XMFLOAT3 luPos;
	XMStoreFloat3(&luPos, leftUp + center);

	DirectX::XMFLOAT3 ruPos;
	XMStoreFloat3(&ruPos, rightUp + center);

	DirectX::XMFLOAT3 ldPos;
	XMStoreFloat3(&ldPos, leftDown + center);

	DirectX::XMFLOAT3 rdPos;
	XMStoreFloat3(&rdPos, rightDown + center);

	vertices.reserve(4);
	vertices.push_back(PrimitiveVertex(luPos, norm, DirectX::XMFLOAT2(0, 0)));
	vertices.push_back(PrimitiveVertex(ruPos, norm, DirectX::XMFLOAT2(1, 0)));
	vertices.push_back(PrimitiveVertex(ldPos, norm, DirectX::XMFLOAT2(0, 1)));
	vertices.push_back(PrimitiveVertex(rdPos, norm, DirectX::XMFLOAT2(1, 1)));

	int size = sizeof(PrimitiveVertex);

	DX12CTRL_INSTANCE
	d12->result = d12->GetDev()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((vertices.size() * sizeof(PrimitiveVertex))),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));
	D12RESULTCHECK

	mapData = nullptr;
	d12->result = vertexBuffer->Map(0,nullptr, (void**)&mapData);
	D12RESULTCHECK

	memcpy(mapData, &vertices[0], vertices.size() * sizeof(PrimitiveVertex));

	vbView = new D3D12_VERTEX_BUFFER_VIEW();
	vbView->BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vbView->SizeInBytes = static_cast<UINT>(sizeof(PrimitiveVertex) * vertices.size());
	vbView->StrideInBytes = sizeof(PrimitiveVertex);
}

PrimitivePlane::~PrimitivePlane()
{
}

void PrimitivePlane::Draw()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	d12->GetCmdList()->IASetVertexBuffers(0, 1, vbView);
	d12->GetCmdList()->DrawInstanced(static_cast<UINT>(vertices.size()), 1, 0, 0);
}
