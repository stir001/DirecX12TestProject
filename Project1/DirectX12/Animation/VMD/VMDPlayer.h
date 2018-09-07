#pragma once
/**
*	@file VMDPlayer
*	@brief VMDのモーションデータ再生クラスの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/7
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
*	@brief VMDのモーションをフレームを基準に計算する
*/
class VMDPlayer : public AnimationPlayer
{
public:
	VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMFLOAT4X4>& boneMat, std::shared_ptr<ConstantBufferObject>& boneConstantBuffer);
	~VMDPlayer();

	/**
	*	計算を止める
	*/
	void Stop();

	/**
	*	@brief 計算を始める
	*	@param[i]	isLoop	ループ処理をするかどうかのフラグ
	*						true:ループ処理をする
	*						false:ループ処理をしない
	*/
	void Play(bool isLoop = false);

	/*
	*	状態に応じて計算をする
	*/
	void Update();

	/**
	*	@brief コンスタントバッファにボーンマトリックスを書き込む
	*	@param[in,out]	matrixBuffer	書き込むコンスタントバッファ
	*/
	void WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer);

	/**
	*	@brief 計算するVMDMotionを設定する
	*	@param[in]	vmd		計算するVMDMotion
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

