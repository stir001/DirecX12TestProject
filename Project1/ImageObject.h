#pragma once
#include <DirectXMath.h>
struct ID3D12DescriptorHeap;
class TextureObj;
class VertexBufferObject;

struct ImageVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class ImageObject
{
	friend class ImageLoader;
private:
	int width;
	int height;
	ImageVertex vertex[4];
	VertexBufferObject* vertexBuffer;
	ID3D12DescriptorHeap* texDescHeap;
	TextureObj* texObj;

	void UpdateBuffer();

	ImageObject(int width, int height, TextureObj* texObj,ID3D12DescriptorHeap* texDescHeap);
public:
	void SetPos(float x, float y, float z);
	void SetPos(DirectX::XMFLOAT3& setPos);
	void Draw();
	~ImageObject();
};

