#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "VertexBufferObject.h"
#include "TextureObj.h"
#include "XMFloat3Operators.h"

DirectX::XMFLOAT3 RotationXY(DirectX::XMFLOAT3& pos, float rad)
{
	DirectX::XMFLOAT3 rtn = {};
	rtn.x = pos.x * cos(rad) - pos.y * sin(rad);
	rtn.y = pos.x * sin(rad) + pos.y * cos(rad);
	return rtn;
}

ImageObject::ImageObject(int inwidth, int inheight, TextureObj* intexObj, ID3D12DescriptorHeap* intexDescHeap) 
	:width(inwidth), height(inheight)
	, vertex{ {{0.f, static_cast<float>(height), 0.f },{0.f, 0.f }}/* v1 */,
		{{ static_cast<float>(width),static_cast<float>(height), 0.f },{1.f, 0.f }}/* v2 */
		,{ {0.0f,0.0f , 0.0f},{0.f, 1.f} }/* v3 */
		,{ { static_cast<float>(width), 0.0f, 0.f },{1.f, 1.f } }/* v4 */ }
	, vertexBuffer(new VertexBufferObject(sizeof(ImageVertex), 4))
	, texObj(intexObj), texDescHeap(intexDescHeap)
	, scale(1.0f), rota(0.0f), center{0.f,0.f,0.f}
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12->GetWindowSize();
	DirectX::XMFLOAT3 offset = { static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f, 0.0f };
	for (int i = 0; i < 4; ++i)
	{
		DirectX::XMFLOAT3 vec = vertex[i].pos - offset;
		normvec[i] = NormalizeXMFloat3(vec);
		length[i] =sqrt(DotXMFloat3(vec, vec));
	}
	SetPos(center);
}

ImageObject::~ImageObject()
{
}

void ImageObject::Draw()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pso_image));
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(rsi_image));
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	vertexBuffer->SetBuffer();
	d12->GetCmdList()->SetDescriptorHeaps(1, &texDescHeap);
	texObj->SetBuffer();
	d12->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void ImageObject::UpdateBuffer()
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12->GetWindowSize();
	for (int i = 0; i < 4; ++i)
	{
		vertex[i].pos = RotationXY(normvec[i], rota) * length[i] * scale + center;
		vertex[i].pos.x /= size.x;
		vertex[i].pos.y /= size.y;
	}
	vertexBuffer->WriteBuffer(&vertex, sizeof(ImageVertex) * 4);
}

void ImageObject::SetPos(float x, float y, float z)
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12->GetWindowSize();
	center.x = x;
	center.y = y;
	center.z = z;

	UpdateBuffer();
}

void ImageObject::SetPos(DirectX::XMFLOAT3& setPos)
{
	SetPos(setPos.x, setPos.y, setPos.z);
}

void ImageObject::SetScale(float s)
{
	scale = s;
	SetPos(center);
}

void ImageObject::SetRota(float deg)
{
	rota = DirectX::XMConvertToRadians(deg);

	UpdateBuffer();
}