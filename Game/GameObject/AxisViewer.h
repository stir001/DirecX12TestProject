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
	*	@brief	描画する
	*/
	void Draw();

	/**
	*	@brief	現在つかんでいる軸がどれかを取得する
	*/
	HoldAxis GetHoldAxis() const;

	/**
	*	@brief	ray当たった座標を得る
	*/
	DirectX::XMFLOAT3 GetRayHitPos() const;

	/**
	*	@brief	軸のUIをつかむ
	*	@return つかんだUIの種類を返す
	*/
	HoldAxis CatchAxisUI(const DirectX::XMFLOAT2& mousePos);

	/**
	*	@brief	UIをつかんでいない状態にする
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

