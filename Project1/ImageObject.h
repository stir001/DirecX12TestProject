#pragma once
#include <DirectXMath.h>
struct ID3D12DescriptorHeap;
class TextureObj;
class VertexBufferObject;
class Rect;

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
	float scale;
	float rota;
	float length[4];
	DirectX::XMFLOAT3 normvec[4];
	DirectX::XMFLOAT3 center;
	ImageVertex vertex[4];
	VertexBufferObject* vertexBuffer;
	ID3D12DescriptorHeap* texDescHeap;
	TextureObj* texObj;

	//画像内の座標で描画範囲を指定する
	Rect* rect;

	void UpdateUV();
	void UpdateNormvec();
	void UpdateBuffer();

	ImageObject(int width, int height, TextureObj* texObj,ID3D12DescriptorHeap* texDescHeap);
public:
	void SetRect(DirectX::XMFLOAT3& inc,float inw, float inh);
	void SetPos(float x, float y, float z);
	void SetPos(DirectX::XMFLOAT3& setPos);
	void SetScale(float s);
	void SetRota(float deg);
	void Draw();
	~ImageObject();
};

