#pragma once
#include "AnimationPlayer.h"

#include <memory>
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
	VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMFLOAT4X4>& boneMat);
	~VMDPlayer();

	void Stop();
	void Play(bool loopFlag = false);
	void Update();
	void WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer);
	void SetVMD(std::shared_ptr<VMDMotion> vmd);
private:
	std::vector<DirectX::XMFLOAT4X4>& mCurrentBoneMatrix;
	std::map<std::string, std::vector<VMDPose>>* mPoses;
	std::vector<PMDBoneData>& mBoneDatas;
	BoneTree& mBoneNode;
	int mFrame;
	int mId;
	bool mIsLoop;
	void (VMDPlayer::*mUpdate)();
	void (VMDPlayer::*mEndCheck)();

	void VMDBoneRotation(const std::string& boneName, DirectX::XMMATRIX& boneRotaMatrix);
	void VMDBoneChildRotation(DirectX::XMFLOAT4X4& parentBoneMatrix, int parentIndex);

	void LoopEndCheck();
	void NonCheck();
	void PlayingUpdate();
	void StopUpdate();
};

