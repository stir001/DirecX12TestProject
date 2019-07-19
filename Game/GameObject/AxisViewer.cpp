#include "AxisViewer.h"
#include "ScreenRayCast.h"
#include <Dx12MSLib.h>


AxisViewer::AxisViewer() : mDrawState(&AxisViewer::DrawAllAxis), mHold(HoldAxis::NONE)
{
	auto& dev = Dx12Ctrl::Instance().GetDev();
	auto rootSignature = std::make_shared<Image3DRootSignature>(dev);
	auto pipelineState = std::make_shared<TransparentAfterPipelineState>("TransAfter", rootSignature, dev);

	mXAxis = LoadImage3D("Game/resource/XAxis.png");
	mXAxis->AddRotaY(DirectX::XM_PIDIV2);
	mXAxisAfter = LoadImage3D("Game/resource/XAxis.png");
	mXAxisAfter->AddRotaY(DirectX::XM_PIDIV2);
	mXAxisAfter->SetGraphicsRootSignature(rootSignature);
	mXAxisAfter->SetPipelineState(pipelineState);

	mYAxis = LoadImage3D("Game/resource/YAxis.png");
	mYAxis->AddRotaX(DirectX::XM_PIDIV2);
	mYAxisAfter = LoadImage3D("Game/resource/YAxis.png");
	mYAxisAfter->AddRotaX(DirectX::XM_PIDIV2);
	mYAxisAfter->SetGraphicsRootSignature(rootSignature);
	mYAxisAfter->SetPipelineState(pipelineState);

	mZAxis = LoadImage3D("Game/resource/ZAxis.png");
	mZAxisAfter = LoadImage3D("Game/resource/ZAxis.png");
	mZAxisAfter->SetGraphicsRootSignature(rootSignature);
	mZAxisAfter->SetPipelineState(pipelineState);

	mCamera = GetCamera();
}


AxisViewer::~AxisViewer()
{
}

void AxisViewer::Draw()
{
	(this->*mDrawState)();
}

AxisViewer::HoldAxis AxisViewer::GetHoldAxis() const
{
	return mHold;
}

DirectX::XMFLOAT3 AxisViewer::GetRayHitPos() const
{
	return mRayHitPos;
}

AxisViewer::HoldAxis AxisViewer::CatchAxisUI(const DirectX::XMFLOAT2& mousePos)
{
	DirectX::XMFLOAT3 origin = { 0,0,0 };

	auto xNormal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	auto xHit = ScreenRayCast::RayCastPlane(mousePos, origin, xNormal);
	float xRadius = GetLengthXMFloat3(xHit.hitPos - origin);

	auto yNormal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	auto yHit = ScreenRayCast::RayCastPlane(mousePos, origin, yNormal);
	float yRadius = GetLengthXMFloat3(yHit.hitPos - origin);

	auto zNormal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	auto zHit = ScreenRayCast::RayCastPlane(mousePos, origin, zNormal);
	float zRadius = GetLengthXMFloat3(zHit.hitPos - origin);

	float length = FLT_MAX;
	const float unit = 20.0f;
	const float inRadius = mXAxis->GetImageSize().x / unit * 0.9f * 0.5f;//³•ûŒ`‚Ì‘å‚«‚³‚ª“¯‚¶‰æ‘œ‚ðŽg‚¤‚Ì‚Åc‰¡‚Í‹C‚É‚µ‚È‚¢
	const float outRadius = mXAxis->GetImageSize().x / unit * 0.5f;
	mHold = HoldAxis::NONE;
	if (xRadius <= outRadius && xRadius >= inRadius)
	{
		length = xHit.length;
		mDrawState = &AxisViewer::DrawXAxis;
		mHold = HoldAxis::X;
		mRayHitPos = xHit.hitPos;
	}
	if (yRadius <= outRadius && yRadius >= inRadius && length >= yHit.length)
	{
		length =yHit.length;
		mDrawState = &AxisViewer::DrawYAxis;
		mHold = HoldAxis::Y;
		mRayHitPos = yHit.hitPos;
	}
	if (zRadius <= outRadius && zRadius >= inRadius && length >= zHit.length)
	{
		mDrawState = &AxisViewer::DrawZAxis;
		mHold = HoldAxis::Z;
		mRayHitPos = zHit.hitPos;
	}

	return mHold;
}

void AxisViewer::ReleaseUI()
{
	mDrawState = &AxisViewer::DrawAllAxis;
	mHold = HoldAxis::NONE;
	mRayHitPos = {FLT_MAX, FLT_MAX, FLT_MAX};
}

void AxisViewer::DrawXAxis()
{
	mXAxis->Draw();
	mXAxisAfter->Draw();
}

void AxisViewer::DrawYAxis()
{
	mYAxis->Draw();
	mYAxisAfter->Draw();
}

void AxisViewer::DrawZAxis()
{
	mZAxis->Draw();
	mZAxisAfter->Draw();
}

void AxisViewer::DrawAllAxis()
{
	mXAxis->Draw();
	mYAxis->Draw();
	mZAxis->Draw();

	mXAxisAfter->Draw();
	mYAxisAfter->Draw();
	mZAxisAfter->Draw();
}
