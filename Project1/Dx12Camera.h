#pragma once
#include <DirectXMath.h>

class ConstantBufferObject;

struct Dx12CameraBuffer
{
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 target;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewproj;
};

class Dx12Camera
{
private:
	Dx12CameraBuffer element;

	DirectX::XMMATRIX camera;//ÉJÉÅÉâÇÃviewçsóÒ
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT3 upper;

	int width;
	int height;
	int buffersize;

	ConstantBufferObject* cameraBuffer;

	void UpdateElement();
	void UpdateBuffer();
public:
	void SetBuffer();
	void SetPos(DirectX::XMFLOAT3& pos);
	void AddXAxisRota(float deg);
	void AddYAxisRota(float deg);
	void AddZAxisRota(float deg);

	void MoveUp(float val);
	void MoveFront(float vel);
	void MoveSide(float vel);
	void TurnRightLeft(float deg);
	void TurnUpDown(float deg);

	Dx12Camera(int wWidth, int wHeight);
	Dx12Camera(int wWidth, int wHeight, DirectX::XMFLOAT3& eye,DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& upper);
	~Dx12Camera();
};
