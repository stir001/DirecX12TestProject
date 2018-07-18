#include "VMDPlayer.h"
#include "VMDMotion.h"
#include "PMDModel.h"
#include "ConstantBufferObject.h"
#include "AnimationPlayerManager.h"
#include "XMFloatOperators.h"

#include <algorithm>

using namespace DirectX;

VMDPlayer::VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMFLOAT4X4>& boneMat)
	:mBoneDatas(bDatas), mBoneNode(node), mFrame(0), mUpdate(&VMDPlayer::StopUpdate), mEndCheck(&VMDPlayer::LoopEndCheck), mId(-1),mCurrentBoneMatrix(boneMat)
{
}

VMDPlayer::~VMDPlayer()
{
}

void VMDPlayer::Update()
{
	(this->*mUpdate)();
}

void VMDPlayer::SetVMD(std::shared_ptr<VMDMotion> vmd)
{
	mPoses = &vmd->mPoses;
	//Play();
}

void VMDPlayer::PlayingUpdate()
{
	DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
	for (auto& cMat : mCurrentBoneMatrix)
	{
		DirectX::XMStoreFloat4x4(&cMat, identity);
	}

	for (auto& p : *mPoses)
	{
		auto findritr = p.second.rbegin();
		for (; findritr != p.second.rend(); findritr++)
		{
			if (findritr->frameNo <= mFrame / 2) break;
		}
		if (findritr == p.second.rbegin())
		{
			
			XMVECTOR v = XMLoadFloat4(&findritr->quoternion);
			XMMATRIX q = XMMatrixRotationQuaternion(v);
			VMDBoneRotation(p.first, q);
		}
		else
		{
			float t = (static_cast<float>(mFrame) * 0.5f - static_cast<float>(findritr->frameNo)) / static_cast<float>(findritr.base()->frameNo - findritr->frameNo);
			XMVECTOR q = XMQuaternionSlerp(XMLoadFloat4(&findritr->quoternion), XMLoadFloat4(&findritr.base()->quoternion), t);
			DirectX::XMMATRIX mat = XMMatrixRotationQuaternion(q);
			VMDBoneRotation(p.first, mat);
		}
	}
	/*for (auto& p : *poses)
	{
		auto rit = std::find_if(p.second.rbegin(), p.second.rend(), [&](Pose& pose) {return pose.frameNo <= mFrame / 2; });
		if (rit == p.second.rbegin())
		{
			BoneRotation(p.first, XMMatrixRotationQuaternion(rit->quoternion));
		}
		else
		{
			float t = (static_cast<float>(mFrame) * 0.5f - static_cast<float>(rit->frameNo)) / static_cast<float>(rit.base()->frameNo - rit->frameNo);
			DirectX::XMVECTOR q = DirectX::XMQuaternionSlerp(rit->quoternion, rit.base()->quoternion, t);
			BoneRotation(p.first, XMMatrixRotationQuaternion(q));
		}
	}*/
	VMDBoneChildRotation(mCurrentBoneMatrix[0], 0);
	++mFrame %= 121;
	(this->*mEndCheck)();
}

void VMDPlayer::StopUpdate()
{

}

void VMDPlayer::VMDBoneRotation(const std::string& boneName, XMMATRIX& boneRotaMatrix)
{
	PMDBoneData* data = nullptr;
	unsigned int i = 0;
	for (; i < mBoneDatas.size(); i++)
	{
		if (mBoneDatas[i].boneName == boneName)
		{
			data = &mBoneDatas[i];
			break;
		}
	}
	if (data == nullptr) return;


	XMVECTOR offsetvec = XMLoadFloat3(&data->pos);
	XMMATRIX boneMat = DirectX::XMMatrixTranslationFromVector(-offsetvec);
	boneMat *= boneRotaMatrix;
	boneMat *= DirectX::XMMatrixTranslationFromVector(offsetvec);
	DirectX::XMStoreFloat4x4(&mCurrentBoneMatrix[i] ,boneMat * DirectX::XMLoadFloat4x4(&mCurrentBoneMatrix[i]));
}

void VMDPlayer::VMDBoneChildRotation(XMFLOAT4X4& parentBoneMatrix, int parentIndex)
{
	for (auto& childIndex : mBoneNode.node[parentIndex])
	{
		mCurrentBoneMatrix[childIndex] *= parentBoneMatrix;
		VMDBoneChildRotation(mCurrentBoneMatrix[childIndex], childIndex);
	}
}

void VMDPlayer::Play(bool flag)
{
	mIsLoop = flag;
	mUpdate = &VMDPlayer::PlayingUpdate;
	mId = AnimationPlayerManager::Instance()->SetAnimation(this);
	if (mIsLoop)
	{
		mEndCheck = &VMDPlayer::NonCheck;
	}
	else
	{
		mEndCheck = &VMDPlayer::LoopEndCheck;
	}
}

void VMDPlayer::LoopEndCheck()
{
	if (mFrame == 0) Stop();
}

void VMDPlayer::NonCheck()
{
}

void VMDPlayer::Stop()
{
	mUpdate = &VMDPlayer::StopUpdate;
	AnimationPlayerManager::Instance()->RemoveAnimation(mId);
	mId = -1;
}

void VMDPlayer::WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer)
{
	matrixBuffer->WriteBuffer(&mCurrentBoneMatrix[0], static_cast<unsigned int>(mCurrentBoneMatrix.size() * sizeof(mCurrentBoneMatrix[0])));
}