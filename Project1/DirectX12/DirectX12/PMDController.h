#pragma once
#include "DrawObjectController.h"
#include <DirectXMath.h>
#include <vector>
#include <memory>

class PMDModel;
class ConstantBufferObject;
class VMDMotion;
class VMDPlayer;
class DirectionalLight;
struct ID3D12DescriptorHeap;

class PMDController
	//: public DrawObjectController
{
	friend class PMDLoader;
public:
	PMDController();
	~PMDController();

	void Draw();
	void SetMotion(VMDMotion* motion);
	void PlayMotion(bool loopFlag = false);
	void StopMotion();
	void SetPosition(DirectX::XMFLOAT3& p);
	void SetRota(DirectX::XMFLOAT3& rota);

	void SetLight(std::shared_ptr<DirectionalLight> dlight);
private:
	PMDModel* mModel;
	DirectX::XMFLOAT3 mPos;
	DirectX::XMFLOAT3 mRotation;
	ConstantBufferObject* mBoneMatrixBuffer;
	std::vector<DirectX::XMMATRIX> mBoneMatrix;
	std::shared_ptr<DirectionalLight> mDirLight;
	VMDPlayer* mVmdPlayer;

	void SetBoneBuffer();
};

