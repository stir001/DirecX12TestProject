#pragma once
#include <vector>
#include <DirectXMath.h>

class IndexBufferObject;
class VertexBufferObject;
class TextureObj;
class ConstantBufferObject;
struct ID3D12DescriptorHeap;

struct Dx12Material
{
	DirectX::XMFLOAT3 diffuse;
	float alpha;
	float specularity;
	DirectX::XMFLOAT3 specular;
	DirectX::XMFLOAT3 ambient;
	DirectX::XMFLOAT3 offset;
};

class Model
{
private:
protected:
	IndexBufferObject* indexBuffer;
	VertexBufferObject* vertexBuffer;
	unsigned int texturecount;
	std::vector<TextureObj*> textureObjects;
	ID3D12DescriptorHeap* textureDescHeap;
	ConstantBufferObject* materialBuffer;
	Dx12Material* d12mat;
public:
	void SetIndexBuffer();
	void SetVertexBuffer();
	virtual void SetMaterialBuffer();

	 Model();
	 virtual ~Model();
};

