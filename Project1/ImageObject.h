#pragma once
#include <DirectXMath.h>
struct ID3D12DescriptorHeap;
class TextureObj;
class VertexBufferObject;
class Rect;

struct ImageVertex;

class ImageObject
{
	friend class ImageLoader;
private:
	int mWidth;
	int mHeight;
	ID3D12DescriptorHeap* mTexDescHeap;
	TextureObj* mTexObj;

	ImageObject(int width, int height, TextureObj* texObj,ID3D12DescriptorHeap* texDescHeap);
public:
	DirectX::XMFLOAT2 GetImageSize();
	void SetImage() const;
	~ImageObject();
};

