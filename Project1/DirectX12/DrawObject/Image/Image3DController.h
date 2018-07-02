#pragma once
#include "DrawObjectController.h"
#include "ImageControllerStructure.h"
#include <DirectXMath.h>

class VertexBufferObject;
class ConstantBufferObject;
class Rect;
class ImageObject;
class Dx12DescriptorHeapObject;

class Image3DController :
	public DrawObjectController
{
public:
	Image3DController(std::shared_ptr<ImageObject> img,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);
	~Image3DController();

	void AddCenterPos(const float x, const float y, const float z);
	void AddCenterPos(const DirectX::XMFLOAT3& offset);
	void AddScale(const float scale);
	void AddScale(const float scaleX, const float scaleY, const float scaleZ);
	void AddRotaX(float deg);
	void AddRotaY(float deg);
	void AddRotaZ(float deg);

	void SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh);
	void SetRect(const Rect& rc);
	void SetCenterPos(const float x, const float y, const float z);
	void SetCenterPos(const DirectX::XMFLOAT3& setPos);
	void SetScale(const float s);
	void SetScale(const float sx, const float sy, const float sz);

	void TurnX();
	void TurnY();
	void Draw();

	DirectX::XMFLOAT2 GetImageSize();
	std::shared_ptr<Image3DController> GetNewCopy();
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);
	void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	std::string GetImageName() const;
	std::string GetFilePath() const;
private:
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
		eROOT_PARAMATER_INDEX_CAMERA,
		eROOT_PARAMATER_INDEX_MATRIX,
	};

	float mScaleX;
	float mScaleY;
	float mScaleZ;

	DirectX::XMFLOAT4X4 mRotaMatrix;
	DirectX::XMFLOAT4X4 mImageMatrix;

	DirectX::XMFLOAT2 mTurnSign;
	DirectX::XMFLOAT3 mCenter;//描画の際の基準点(画面座標)回転や反転の中心
	DirectX::XMFLOAT4 mNormal;
	Image3DVertex mVertex[4];
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<ConstantBufferObject> mImageMatrixBuffer;
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;
	Rect* mRect;//画像内の切り抜きローカル座標矩形

	std::shared_ptr<ImageObject> mImgObj;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	void (Image3DController::*mBundleUpdate)();

	void UpdateUV();
	void UpdateMatrix();
	void UpdateBuffer();
	void UpdateBundle();
	void NonUpdateBundle();
};

