#pragma once
#include "ModelController.h"
#include <DirectXMath.h>
#include <vector>
#include <memory>

class PMDModel;
class ConstantBufferObject;
class VMDMotion;
class VMDPlayer;
class DirectionalLight;
struct ID3D12DescriptorHeap;

class PMDController :
	public ModelController
{
	friend class PMDLoader;
private:
	PMDModel* model;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rotation;
	ConstantBufferObject* boneMatrixBuffer;
	std::vector<DirectX::XMMATRIX> boneMatrix;
	std::shared_ptr<DirectionalLight> dirLight;
	VMDPlayer* vmdPlayer;

	void SetBoneBuffer();
public:
	void Draw();
	void SetMotion(VMDMotion* motion);
	void PlayMotion(bool loopFlag = false);
	void StopMotion();
	void SetPosition(DirectX::XMFLOAT3& p);
	void SetRota(DirectX::XMFLOAT3& rota);

	void SetLight(std::shared_ptr<DirectionalLight> dlight);

	PMDController();
	~PMDController();
};

