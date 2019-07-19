#pragma once
#include <Dx12MSLib.h>



class VertexCustumController: public DrawController3D
{
public:
	VertexCustumController();
	~VertexCustumController();

	/**
	*	@brief	頂点を設定する
	*/
	void SetVerts(std::vector<PrimitiveVertex>& verts);

	/**
	*	@brief	インデックスを設定する
	*/
	void SetIndices(std::vector<unsigned int>& indices);

	/**
	*	@brief	
	*/
	void Draw();

	void UpdateDescriptorHeap();
private:

	struct CBuffer
	{
		DirectX::XMFLOAT4 color;
	};

	enum RootParamaterIndex
	{
		CAMERA,
		MATRIX,
		CBUFFER
	};

	void BundleUpdate();

	void UpdateDescHeap();
	void NonUpdate();

	std::vector<PrimitiveVertex> mVerts;
	std::vector<unsigned int> mIndices;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<IndexBufferObject> mIndexBuffer;
	std::shared_ptr<ConstantBufferObject> mConstantBuffer;

	void (VertexCustumController::*mUpdateDescHeap)();
	CBuffer mBuffer;
};

