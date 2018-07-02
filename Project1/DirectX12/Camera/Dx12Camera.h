#pragma once
#include <DirectXMath.h>
#include <wrl.h	>
#include <memory>

class ConstantBufferObject;
class DxInput;
struct ID3D12GraphicsCommandList;

struct Dx12CameraBuffer
{
	DirectX::XMFLOAT4 eye;
	DirectX::XMFLOAT4 target;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

class Dx12Camera
{
public:
	Dx12Camera(int wWidth, int wHeight);
	Dx12Camera(int wWidth, int wHeight, DirectX::XMFLOAT3& eye, DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& upper);
	~Dx12Camera();

	void SetPos(DirectX::XMFLOAT3& pos);
	void SetTarget(DirectX::XMFLOAT3& target);
	void AddXAxisRota(float deg);
	void AddYAxisRota(float deg);
	void AddZAxisRota(float deg);

	void MoveUp(float val);
	void MoveFront(float vel);
	void MoveSide(float vel);
	void TurnRightLeft(float deg);
	void TurnUpDown(float deg);

	void SetViewPort();
	void SetScisorRect();
	void DefaultMove(const DxInput& input);

	DirectX::XMFLOAT4X4 GetView();
	DirectX::XMFLOAT4X4 GetProjection();
	DirectX::XMFLOAT4 GetCameraPosition();
	DirectX::XMFLOAT4X4 GetWorld();

	std::shared_ptr<ConstantBufferObject>& GetCameraBuffer();
private:
	Dx12CameraBuffer mElement;
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;

	DirectX::XMFLOAT4X4 mCamera;//ÉJÉÅÉâÇÃviewçsóÒ
	DirectX::XMFLOAT4X4 mProjection;
	DirectX::XMFLOAT3 mUpper;
	DirectX::XMFLOAT4X4 mWorldRotation;
	DirectX::XMFLOAT3 mLocalUpper;

	int mWidth;
	int mHeight;
	int mBuffersize;

	void AddRotationAxis(DirectX::XMMATRIX& rotaMatrix);

	DirectX::XMFLOAT3 GetLocalUpper();
	DirectX::XMFLOAT3 GetEyeToTargetVec();
	void UpdateElement();
	void UpdateBuffer();
};
