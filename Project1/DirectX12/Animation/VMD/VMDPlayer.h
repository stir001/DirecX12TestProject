#pragma once
#include "AnimationPlayer.h"

#include <DirectXMath.h>
#include <vector>
#include <map>
#include <string>

class VMDMotion;
class ConstantBufferObject;
struct VMDPose;
struct PMDBoneData;
struct BoneTree;

class VMDPlayer : public AnimationPlayer
{
public:
	VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMMATRIX>& boneMat);
	~VMDPlayer();

	void Stop();
	void Play(bool loopFlag = false);
	void Update();
	void WriteBoneMatrix(ConstantBufferObject* matrixBuffer);
	void SetVMD(VMDMotion* vmd);
private:
	std::vector<DirectX::XMMATRIX>& currentBoneMatrix;
	std::map<std::string, std::vector<VMDPose>>* poses;
	std::vector<PMDBoneData>& boneDatas;
	BoneTree& boneNode;
	int frame;
	int id;
	bool loopFlag;
	void VMDBoneRotation(const std::string& boneName, DirectX::XMMATRIX& boneRotaMatrix);
	void VMDBoneChildRotation(DirectX::XMMATRIX& parentBoneMatrix,int parentIndex);
	void (VMDPlayer::*update)();
	void (VMDPlayer::*endCheck)();

	void LoopEndCheck();
	void NonCheck();
	void PlayingUpdate();
	void StopUpdate();
};

