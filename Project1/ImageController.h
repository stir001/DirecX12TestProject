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
	DirectX::XMFLOAT3 mPivotOffset;//�`�悷��摜�̒��S����̕`���_�փI�t�Z�b�g
	DirectX::XMFLOAT3 mNormvec[4];
	DirectX::XMFLOAT3 mCenter;//�`��̍ۂ̊�_(��ʍ��W)
	ImageVertex mVertex[4];
	VertexBufferObject* mVertexBuffer;
	Rect* mRect;//�摜���̐؂蔲�����[�J�����W

	std::shared_ptr<ImageObject> mImgObj;

	void UpdateUV();
	void UpdateNormvec();
	void UpdateBuffer();
};

