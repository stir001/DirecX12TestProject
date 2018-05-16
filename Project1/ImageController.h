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
	void SetRota(const float deg);
	void SetCenterOffset(const float x, const float y, const float z);
	void SetCenterOffset(const DirectX::XMFLOAT3& offset);
	void TurnX();
	void TurnY();
	void Draw() const;

	DirectX::XMFLOAT2 GetImageSize();
	std::shared_ptr<ImageController> GetNewCopy();
private:
	float mScale;
	float mRota;
	float mLength[4];
	DirectX::XMFLOAT2 mTurnSign;
	DirectX::XMFLOAT3 mCenterOffset;//�`�悷��摜�̒��S�̕`���_����̃I�t�Z�b�g
	DirectX::XMFLOAT3 mNormvec[4];
	DirectX::XMFLOAT3 mPivot;//�`��̍ۂ̊�_(��ʍ��W)��]�┽�]�̒��S
	ImageVertex mVertex[4];
	VertexBufferObject* mVertexBuffer;
	Rect* mRect;//�摜���̐؂蔲�����[�J�����W��`

	std::shared_ptr<ImageObject> mImgObj;

	void UpdateUV();
	void UpdateNormvec();
	void UpdateBuffer();
};

