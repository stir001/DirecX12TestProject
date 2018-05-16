#include "ImageController.h"
#include "Dx12Ctrl.h"
#include "XMFloat3Operators.h"
#include "ImageObject.h"
#include "VertexBufferObject.h"
#include "Rect.h"

DirectX::XMFLOAT3 RotationXY(DirectX::XMFLOAT3& pos, float rad)
{
	DirectX::XMFLOAT3 rtn = {};
	rtn.x = pos.x * cos(rad) - pos.y * sin(rad);
	rtn.y = pos.x * sin(rad) + pos.y * cos(rad);
	return rtn;
}

ImageController::ImageController(std::shared_ptr<ImageObject> img):mImgObj(img)
	, mVertex{ { { 0.f, img->GetImageSize().y, 0.f },{ 0.f, 0.f } }/* v1 */,
{ { img->GetImageSize().x,img->GetImageSize().y, 0.f },{ 1.f, 0.f } }/* v2 */
,{ { 0.0f,0.0f , 0.0f },{ 0.f, 1.f } }/* v3 */
,{ { img->GetImageSize().x, 0.0f, 0.f },{ 1.f, 1.f } }/* v4 */ }
	, mVertexBuffer(new VertexBufferObject(sizeof(ImageVertex), 4))
	, mScale(1.0f), mRota(0.0f), mPivot{ 0.f,0.f,0.f }, mCenterOffset(0,0,0)
	, mRect(new Rect(mPivot, img->GetImageSize().x, img->GetImageSize().y))
	, mTurnSign(1,1)
{
	DirectX::XMFLOAT2 size = mImgObj->GetImageSize();
	DirectX::XMFLOAT3 offset = { size.x / 2.0f,size.y / 2.0f, 0.0f };
	for (int i = 0; i < 4; ++i)
	{
		DirectX::XMFLOAT3 vec = mVertex[i].pos - offset;
		mNormvec[i] = NormalizeXMFloat3(vec);
		mLength[i] = sqrt(DotXMFloat3(vec, vec));
	}
	SetPos(mPivot);
}

ImageController::~ImageController()
{
	delete(mVertexBuffer);
	delete(mRect);
}

void ImageController::AddPos(const float x, const float y, const float z)
{
	mPivot.x = x;
	mPivot.y = y;
	mPivot.z = z;
	UpdateBuffer();
}

void ImageController::AddPos(const DirectX::XMFLOAT3& offset)
{
	AddPos(offset.x, offset.y, offset.z);
}

void ImageController::AddRota(const float deg)
{
	mRota += DirectX::XMConvertToRadians(deg);
	UpdateBuffer();
}

void ImageController::AddScale(const float scale)
{
	mScale += scale;
	UpdateBuffer();
}

void ImageController::AddPivot(const DirectX::XMFLOAT3& offset)
{

}

void ImageController::SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh)
{
	mRect->SetCenter(inc);
	mRect->SetHeight(inh);
	mRect->SetWidth(inw);
	UpdateNormvec();
	UpdateUV();
	UpdateBuffer();
}

void ImageController::SetRect(const Rect& rc)
{
	SetRect(rc.GetCenter(), rc.GetWidth(), rc.GetHeight());
}

void ImageController::SetPos(const float x, const float y, const float z)
{
	mPivot.x = x;
	mPivot.y = y;
	mPivot.z = z;

	UpdateBuffer();
}

void ImageController::SetPos(const DirectX::XMFLOAT3& setPos)
{
	SetPos(setPos.x, setPos.y, setPos.z);
}

void ImageController::SetScale(const float s)
{
	mScale = s;
	UpdateBuffer();
}

void ImageController::SetRota(const float deg)
{
	mRota = DirectX::XMConvertToRadians(deg);
	UpdateBuffer();
}

void ImageController::SetCenterOffset(const float x, const float y, const float z)
{
	mCenterOffset.x = x;
	mCenterOffset.y = y;
	mCenterOffset.z = z;
	UpdateNormvec();
	UpdateBuffer();
}

void ImageController::SetCenterOffset(const DirectX::XMFLOAT3& offset)
{
	SetCenterOffset(offset.x, offset.y, offset.z);
}

void ImageController::TurnX()
{
	DirectX::XMFLOAT2 uv;
	uv = mVertex[0].uv;
	mVertex[0].uv = mVertex[1].uv;
	mVertex[1].uv = uv;

	uv = mVertex[2].uv;
	mVertex[2].uv = mVertex[3].uv;
	mVertex[3].uv = uv;

	mTurnSign.x *= -1;
	UpdateNormvec();
	UpdateBuffer();
}

void ImageController::TurnY()
{
	DirectX::XMFLOAT2 uv;
	uv = mVertex[0].uv;
	mVertex[0].uv = mVertex[2].uv;
	mVertex[2].uv = uv;

	uv = mVertex[1].uv;
	mVertex[1].uv = mVertex[3].uv;
	mVertex[3].uv = uv;

	mTurnSign.y *= -1;
	UpdateNormvec();
	UpdateBuffer();
}

void ImageController::Draw() const
{
	DX12CTRL_INSTANCE
	d12->GetCmdList()->SetPipelineState(d12->GetPiplineState(pso_image));
	d12->GetCmdList()->SetGraphicsRootSignature(d12->GetRootSignature(rsi_image));
	d12->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mVertexBuffer->SetBuffer();
	mImgObj->SetImage();
	d12->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

DirectX::XMFLOAT2 ImageController::GetImageSize()
{
	return mImgObj->GetImageSize();
}

std::shared_ptr<ImageController> ImageController::GetNewCopy()
{
	std::shared_ptr<ImageController> rtn(new ImageController(mImgObj));
	return rtn;
}

void ImageController::UpdateUV()
{
	DirectX::XMFLOAT2 size = mImgObj->GetImageSize();
	DirectX::XMFLOAT2 leftupUV = { mRect->GetLeft() / size.x,mRect->GetDown() / size.y };
	DirectX::XMFLOAT2 rightdownUV = { mRect->GetRight() /size.x, mRect->GetUp() / size.y };
	mVertex[0].uv = leftupUV;

	mVertex[1].uv.x = rightdownUV.x;
	mVertex[1].uv.y = leftupUV.y;

	mVertex[2].uv.x = leftupUV.x;
	mVertex[2].uv.y = rightdownUV.y;

	mVertex[3].uv = rightdownUV;

	if (mTurnSign.x == -1)
	{
		DirectX::XMFLOAT2 uv;
		uv = mVertex[0].uv;
		mVertex[0].uv = mVertex[1].uv;
		mVertex[1].uv = uv;

		uv = mVertex[2].uv;
		mVertex[2].uv = mVertex[3].uv;
		mVertex[3].uv = uv;
	}
	if (mTurnSign.y == -1)
	{
		DirectX::XMFLOAT2 uv;
		uv = mVertex[0].uv;
		mVertex[0].uv = mVertex[2].uv;
		mVertex[2].uv = uv;

		uv = mVertex[1].uv;
		mVertex[1].uv = mVertex[3].uv;
		mVertex[3].uv = uv;
	}
}

void ImageController::UpdateNormvec()
{
	mVertex[0].pos.x = mRect->GetLeft() + mCenterOffset.x * mTurnSign.x;
	mVertex[0].pos.y = mRect->GetUp()	+ mCenterOffset.y * mTurnSign.y;

	mVertex[1].pos.x = mRect->GetRight() + mCenterOffset.x * mTurnSign.x;
	mVertex[1].pos.y = mRect->GetUp() + mCenterOffset.y * mTurnSign.y;

	mVertex[2].pos.x = mRect->GetLeft() + mCenterOffset.x * mTurnSign.x;
	mVertex[2].pos.y = mRect->GetDown() + mCenterOffset.y * mTurnSign.y;

	mVertex[3].pos.x = mRect->GetRight() + mCenterOffset.x * mTurnSign.x;
	mVertex[3].pos.y = mRect->GetDown() + mCenterOffset.y * mTurnSign.y;

	const DirectX::XMFLOAT3 offset = mRect->GetCenter();
	for (int i = 0; i < 4; ++i)
	{
		DirectX::XMFLOAT3 vec = mVertex[i].pos - offset;
		mNormvec[i] = NormalizeXMFloat3(vec);
		mLength[i] = sqrt(DotXMFloat3(vec, vec));
	}
}


void ImageController::UpdateBuffer()
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12->GetWindowSize();
	for (int i = 0; i < 4; ++i)
	{
		mVertex[i].pos = RotationXY(mNormvec[i], mRota) * mLength[i] * mScale + mPivot;
		mVertex[i].pos.x *= 2.0f / size.x;
		mVertex[i].pos.y *= 2.0f / size.y;
	}
	mVertexBuffer->WriteBuffer(mVertex, sizeof(ImageVertex) * 4);
}
