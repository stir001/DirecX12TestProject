#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "VertexBufferObject.h"
#include "TextureObj.h"
#include "XMFloat3Operators.h"

void RotationXY(DirectX::XMFLOAT3& pos, float rad)
{
	DirectX::XMFLOAT3 rtn = {};
	rtn.x = pos.x * cos(rad) - pos.y * sin(rad);
	rtn.y = pos.x * sin(rad) + pos.y * cos(rad);
	pos = rtn;
}

ImageObject::ImageObject(int inwidth, int inheight, TextureObj* intexObj, ID3D12DescriptorHeap* intexDescHeap) :width(inwidth), height(inheight)
, vertex{ {{0.f, 0.f, 0.f },{0.f, 0.f }}/* v1 */, {{ static_cast<float>(width), 0.f, 0.f },{1.f, 0.f }}/* v2 */,{ {0.0f, static_cast<float>(-height), 0.0f},{0.f, 1.f} }/* v3 */,{ { static_cast<float>(width), static_cast<float>(-height), 0.f },{1.f, 1.f } }/* v4 */ }
, vertexBuffer(new VertexBufferObject(sizeof(ImageVertex), 4))
, texObj(intexObj), texDescHeap(intexDescHeap)
, scale(1.0f), rota(0.0f), center{ static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f, 0.0f }
{
	SetPos(vertex[0].pos);
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
	vertexBuffer->WriteBuffer(&vertex, sizeof(ImageVertex) * 4);
}

void ImageObject::SetPos(float x, float y, float z)
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12->GetWindowSize();
	center.x = x;
	center.y = y;
	center.z = z;
	DirectX::XMFLOAT2 halfsize = { static_cast<float>(width) * scale/ 2.0f , static_cast<float>(height) * scale / 2.0f };
	vertex[0].pos.x = (center.x - halfsize.x) / size.x;
	vertex[0].pos.y = (center.y + halfsize.y) / size.y;
	vertex[0].pos.z = z;

	vertex[1].pos.x = (center.x + halfsize.x) / size.x;
	vertex[1].pos.y = (center.y + halfsize.y) / size.y;
	vertex[1].pos.z = z;

	vertex[2].pos.x = (center.x - halfsize.x) / size.x;
	vertex[2].pos.y = (center.y - halfsize.y) / size.y;
	vertex[2].pos.z = z;

	vertex[3].pos.x = (center.x + halfsize.x) / size.x;
	vertex[3].pos.y = (center.y - halfsize.y) / size.y;
	vertex[3].pos.z = z;

	float tmprota = rota;
	rota = 0.0f;
	SetRota(DirectX::XMConvertToDegrees(tmprota));

	//UpdateBuffer();
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
	float subrota = DirectX::XMConvertToRadians(deg) - rota;
	rota = DirectX::XMConvertToRadians(deg);
	DirectX::XMFLOAT3 t_vec;
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12->GetWindowSize();
	DirectX::XMFLOAT3 normCenter = { center.x / size.x , center.y / size.y, center.z };
	for (int i = 0; i < 4; ++i)
	{
		t_vec = vertex[i].pos - normCenter;
		float beforelen = DotXMFloat3(t_vec , t_vec);
		RotationXY(t_vec, subrota);
		float afterlen =DotXMFloat3( t_vec , t_vec);
		if (beforelen != afterlen)
		{
			float sublen = beforelen - afterlen;
			bool change = true;
		}
		vertex[i].pos = normCenter + t_vec;
	}
	UpdateBuffer();
}