#pragma once
#include <vector>
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

class IndexBufferObject;
class VertexBufferObject;
class TextureObject;
class ConstantBufferObject;
struct ID3D12DescriptorHeap;
struct ID3D12GraphicsCommandList;
class DrawObjectLoader;

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
public:
	Model();
	virtual ~Model();

	void SetIndexBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;
	void SetVertexBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;
	std::unique_ptr<IndexBufferObject>& GetIndexBuffer() const;
	std::unique_ptr<VertexBufferObject>& GetVertexBuffer() const;
	std::vector<std::shared_ptr<TextureObject>>& GetTextureObjects() const;
	std::shared_ptr<ConstantBufferObject>& GetMaterialBuffer() const;

	const std::string& GetModelName() const;
	const std::string& GetModelPath() const;
protected:
	std::unique_ptr<IndexBufferObject> mIndexBuffer;
	std::unique_ptr<VertexBufferObject> mVertexBuffer;
	unsigned int mTexturecount;
	std::vector<std::shared_ptr<TextureObject>> mTextureObjects;
	std::shared_ptr<ConstantBufferObject> mMaterialBuffer;
	Dx12Material* mD12mat;

	std::string mModelName;
	std::string mModelPath;
private:
	
};

