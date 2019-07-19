#pragma once
#include <memory>
#include <tuple>
#include <DirectXMath.h>

class Image3DController;
class DxInput;
class Dx12Camera;
class ScreenRayCast;

class AxisViewer
{
public:
	AxisViewer();
	~AxisViewer();

	enum class HoldAxis
	{
		NONE,
		X,
		Y,
		Z
	};

	/**
	*	@brief	•`‰æ‚·‚é
	*/
	void Draw();

	/**
	*	@brief	Œ»İ‚Â‚©‚ñ‚Å‚¢‚é²‚ª‚Ç‚ê‚©‚ğæ“¾‚·‚é
	*/
	HoldAxis GetHoldAxis() const;

	/**
	*	@brief	ray“–‚½‚Á‚½À•W‚ğ“¾‚é
	*/
	DirectX::XMFLOAT3 GetRayHitPos() const;

	/**
	*	@brief	²‚ÌUI‚ğ‚Â‚©‚Ş
	*	@return ‚Â‚©‚ñ‚¾UI‚Ìí—Ş‚ğ•Ô‚·
	*/
	HoldAxis CatchAxisUI(const DirectX::XMFLOAT2& mousePos);

	/**
	*	@brief	UI‚ğ‚Â‚©‚ñ‚Å‚¢‚È‚¢ó‘Ô‚É‚·‚é
	*/
	void ReleaseUI();
private:


	void DrawXAxis();
	void DrawYAxis();
	void DrawZAxis();
	void DrawAllAxis();

	void (AxisViewer::*mDrawState)();

	std::shared_ptr<Image3DController> mXAxis;
	std::shared_ptr<Image3DController> mXAxisAfter;
	std::shared_ptr<Image3DController> mYAxis;
	std::shared_ptr<Image3DController> mYAxisAfter;
	std::shared_ptr<Image3DController> mZAxis;
	std::shared_ptr<Image3DController> mZAxisAfter;
	std::shared_ptr<Dx12Camera> mCamera;
	HoldAxis mHold;
	DirectX::XMFLOAT3 mRayHitPos;
};

