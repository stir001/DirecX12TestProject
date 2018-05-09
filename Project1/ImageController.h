#pragma once
#include <memory>
#include <DirectXMath.h>

class ImageObject;
class Rect;
class VertexBufferObject;

struct ImageVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class ImageController
{
public:
	ImageController(std::shared_ptr<ImageObject> img);
	~ImageController();

	void SetRect(DirectX::XMFLOAT3& inc, float inw, float inh);
	void SetPos(float x, float y, float z);
	void SetPos(DirectX::XMFLOAT3& setPos);
	void SetScale(float s);
	void SetRota(float deg);
	void SetPivot(DirectX::XMFLOAT3& offset);
	void TurnX();
	void TurnY();
	void Draw();
private:
	float mScale;
	float mRota;
	float mLength[4];
	DirectX::XMFLOAT2 mTurnSign;
	DirectX::XMFLOAT3 mPivotOffset;//描画する画像の中心からの描画基準点へオフセット
	DirectX::XMFLOAT3 mNormvec[4];
	DirectX::XMFLOAT3 mCenter;//描画の際の基準点(画面座標)
	ImageVertex mVertex[4];
	VertexBufferObject* mVertexBuffer;
	Rect* mRect;//画像内の切り抜きローカル座標

	std::shared_ptr<ImageObject> mImgObj;

	void UpdateUV();
	void UpdateNormvec();
	void UpdateBuffer();
};

