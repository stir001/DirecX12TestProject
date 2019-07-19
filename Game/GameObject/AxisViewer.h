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
	*	@brief	�`�悷��
	*/
	void Draw();

	/**
	*	@brief	���݂���ł��鎲���ǂꂩ���擾����
	*/
	HoldAxis GetHoldAxis() const;

	/**
	*	@brief	ray�����������W�𓾂�
	*/
	DirectX::XMFLOAT3 GetRayHitPos() const;

	/**
	*	@brief	����UI������
	*	@return ����UI�̎�ނ�Ԃ�
	*/
	HoldAxis CatchAxisUI(const DirectX::XMFLOAT2& mousePos);

	/**
	*	@brief	UI������ł��Ȃ���Ԃɂ���
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

