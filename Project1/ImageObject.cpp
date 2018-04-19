#include "ImageObject.h"
#include "Dx12Ctrl.h"
#include "VertexBufferObject.h"
#include "TextureObj.h"

ImageObject::ImageObject(int inwidth, int inheight, TextureObj* intexObj, ID3D12DescriptorHeap* intexDescHeap) :width(inwidth), height(inheight)
, vertex{ {{0.f, 0.f, 0.f },{0.f, 0.f }}/* v1 */, {{ static_cast<float>(width), 0.f, 0.f },{1.f, 0.f }}/* v2 */,{ {0.0f, static_cast<float>(height), 0.0f},{0.f, 1.f} }/* v3 */,{ { static_cast<float>(width), static_cast<float>(height), 0.f },{1.f, 1.f } }/* v4 */ }
, vertexBuffer(new VertexBufferObject(sizeof(ImageVertex), 4))
,texObj(intexObj), texDescHeap(intexDescHeap)
{
	UpdateBuffer();
}

ImageObject::~ImageObject()
{
}

void ImageObject::Draw()
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pso_image));
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(1));
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	vertexBuffer->SetBuffer();
	texObj->SetBuffer();
	d12->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void ImageObject::UpdateBuffer()
{
	vertexBuffer->WriteBuffer(&vertex, sizeof(ImageVertex) * 4);
}

void ImageObject::SetPos(float x, float y, float z)
{
	vertex[0].pos.x = x;
	vertex[0].pos.y = y;
	vertex[0].pos.z = z;

	vertex[1].pos.x = x + width;
	vertex[1].pos.y = y;
	vertex[1].pos.z = z;

	vertex[2].pos.x = x;
	vertex[2].pos.y = y + height;
	vertex[2].pos.z = z;

	vertex[3].pos.x = x + width;
	vertex[3].pos.y = y + height;
	vertex[3].pos.z = z;

	UpdateBuffer();
}

void ImageObject::SetPos(DirectX::XMFLOAT3& setPos)
{
	SetPos(setPos.x, setPos.y, setPos.z);
}