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
	//float scale;
	//float rota;
	//float length[4];
	//DirectX::XMFLOAT3 normvec[4];
	//DirectX::XMFLOAT3 center;
	//ImageVertex vertex[4];
	//VertexBufferObject* vertexBuffer;
	ID3D12DescriptorHeap* mTexDescHeap;
	TextureObj* mTexObj;

	//âÊëúì‡ÇÃç¿ïWÇ≈ï`âÊîÕàÕÇéwíËÇ∑ÇÈ
	//Rect* rect;

	//void UpdateUV();
	//void UpdateNormvec();
	/*void UpdateBuffer();*/

	ImageObject(int width, int height, TextureObj* texObj,ID3D12DescriptorHeap* texDescHeap);
public:
	DirectX::XMFLOAT2 GetImageSize();
	//void SetRect(DirectX::XMFLOAT3& inc,float inw, float inh);
	//void SetPos(float x, float y, float z);
	//void SetPos(DirectX::XMFLOAT3& setPos);
	//void SetScale(float s);
	//void SetRota(float deg);
	//void Turn();
	//void Draw();
	void SetImage() const;
	~ImageObject();
};

