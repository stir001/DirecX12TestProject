#pragma once
/**
*	@file VMDPlayer
*	@brief VMD�̃��[�V�����f�[�^�Đ��N���X�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/7
*/

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

/**
*	@ingroup PMD_Animation
*	@class VMDPlayer
*	@brief VMD�̃��[�V�������t���[������Ɍv�Z����
*/
class VMDPlayer : public AnimationPlayer
{
public:
	VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMFLOAT4X4>& boneMat, std::shared_ptr<ConstantBufferObject>& boneConstantBuffer);
	~VMDPlayer();

	/**
	*	�v�Z���~�߂�
	*/
	void Stop();

	/**
	*	@brief �v�Z���n�߂�
	*	@param[i]	isLoop	���[�v���������邩�ǂ����̃t���O
	*						true:���[�v����������
	*						false:���[�v���������Ȃ�
	*/
	void Play(bool isLoop = false);

	/*
	*	��Ԃɉ����Čv�Z������
	*/
	void Update();

	/**
	*	@brief �R���X�^���g�o�b�t�@�Ƀ{�[���}�g���b�N�X����������
	*	@param[in,out]	matrixBuffer	�������ރR���X�^���g�o�b�t�@
	*/
	void WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer);

	/**
	*	@brief �v�Z����VMDMotion��ݒ肷��
	*	@param[in]	vmd		�v�Z����VMDMotion
	*/
	void SetVMD(std::shared_ptr<VMDMotion>& vmd);
private:
	/**
	*	
	*/
	std::vector<DirectX::XMFLOAT4X4>& mCurrentBoneMatrix;
	std::shared_ptr<ConstantBufferObject>& mBoneConstantBuffer;
	std::map<std::string, std::vector<VMDPose>>* mPoses;
	std::vector<PMDBoneData>& mBoneDatas;
	BoneTree& mBoneNode;
	int mFrame;
	int mMaxFrame;
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

