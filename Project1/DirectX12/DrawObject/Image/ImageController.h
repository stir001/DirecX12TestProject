#pragma once
#include "DrawObjectController.h"
#include "ImageControllerStructure.h"

#include <memory>
#include <DirectXMath.h>

class ImageObject;
class Rect;
class VertexBufferObject;
class Dx12DescriptorHeapObject;


class ImageController :public DrawObjectController
{
public:
	ImageController(std::shared_ptr<ImageObject> img,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList,
		std::shared_ptr<PipelineStateObject>& pipelinestate,
		std::shared_ptr<RootSignatureObject>& rootsignature);
	~ImageController();

	void AddPos(const float x, const float y, const float z);
	void AddPos(const DirectX::XMFLOAT3& offset);
	void AddScale(const float scale);
	void AddRota(const float deg);
	void AddPivot(const DirectX::XMFLOAT3& offset);

	void SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh);
	void SetRect(const Rect& rc);
	void SetPos(const float x, const float y, const float z);
	void SetPos(const DirectX::XMFLOAT3& setPos);
	void SetScale(const float s);
	void SetScale(const DirectX::XMFLOAT2& scale);
	void SetRota(const float deg);
	void SetCenterOffset(const float x, const float y, const float z);
	void SetCenterOffset(const DirectX::XMFLOAT3& offset);
	void TurnX();
	void TurnY();
	void Draw();
	bool IsTurnX() const;
	bool IsTurnY() const;

	DirectX::XMFLOAT2 GetImageSize();
	std::shared_ptr<ImageController> GetNewCopy();
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);
	void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	std::string GetImageName() const;
	std::string GetFilePath() const;
private:
	float mScaleX;
	float mScaleY;
	float mRota;
	float mLength[4];
	DirectX::XMFLOAT2 mTurnSign;
	DirectX::XMFLOAT3 mCenterOffset;//描画する画像の中心の描画基準点からのオフセット
	DirectX::XMFLOAT3 mNormvec[4];
	DirectX::XMFLOAT3 mPivot;//描画の際の基準点(画面座標)回転や反転の中心
	ImageVertex mVertex[4];
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	Rect* mRect;//画像内の切り抜きローカル座標矩形

	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
	};

	std::shared_ptr<ImageObject> mImgObj;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	void (ImageController::*mBundleUpdate)();

	void UpdateUV();
	void UpdateNormvec();
	void UpdateBuffer();
	void UpdateBundle();
	void NonUpdateBundle();
};

