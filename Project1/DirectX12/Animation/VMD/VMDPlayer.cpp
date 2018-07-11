#include "VMDPlayer.h"
#include "VMDMotion.h"
#include "PMDModel.h"
#include "ConstantBufferObject.h"
#include "AnimationPlayerManager.h"

#include <algorithm>

using namespace DirectX;

VMDPlayer::VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMMATRIX>& boneMat)
	:boneDatas(bDatas), boneNode(node), frame(0), update(&VMDPlayer::StopUpdate), endCheck(&VMDPlayer::LoopEndCheck), id(-1),currentBoneMatrix(boneMat)
{
}

VMDPlayer::~VMDPlayer()
{
}

void VMDPlayer::Update()
{
	(this->*update)();
}

void VMDPlayer::SetVMD(std::shared_ptr<VMDMotion> vmd)
{
	poses = &vmd->mPoses;
	//Play();
}

void VMDPlayer::PlayingUpdate()
{
	DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
	unsigned int size = currentBoneMatrix.size();
	for (unsigned int i = 0; i < size; i++)
	{
		currentBoneMatrix[i] = identity;
	}
	auto bitr = poses->begin();
	auto eitr = poses->end();
	for (; bitr != eitr; bitr++)
	{
		auto findritr = bitr->second.rbegin();
		for (; findritr != bitr->second.rend(); findritr++)
		{
			if (findritr->frameNo <= frame / 2) break;
		}
		if (findritr == bitr->second.rbegin())
		{
			
			XMVECTOR v = XMLoadFloat4(&findritr->quoternion);
			XMMATRIX q = XMMatrixRotationQuaternion(v);
			VMDBoneRotation(bitr->first, q);
		}
		else
		{
			float t = (static_cast<float>(frame) * 0.5f - static_cast<float>(findritr->frameNo)) / static_cast<float>(findritr.base()->frameNo - findritr->frameNo);
			XMVECTOR q = XMQuaternionSlerp(XMLoadFloat4(&findritr->quoternion), XMLoadFloat4(&findritr.base()->quoternion), t);
			DirectX::XMMATRIX mat = XMMatrixRotationQuaternion(q);
			VMDBoneRotation(bitr->first, mat);
		}
	}
	/*for (auto& p : *poses)
	{
		auto rit = std::find_if(p.second.rbegin(), p.second.rend(), [&](Pose& pose) {return pose.frameNo <= frame / 2; });
		if (rit == p.second.rbegin())
		{
			BoneRotation(p.first, XMMatrixRotationQuaternion(rit->quoternion));
		}
		else
		{
			float t = (static_cast<float>(frame) * 0.5f - static_cast<float>(rit->frameNo)) / static_cast<float>(rit.base()->frameNo - rit->frameNo);
			DirectX::XMVECTOR q = DirectX::XMQuaternionSlerp(rit->quoternion, rit.base()->quoternion, t);
			BoneRotation(p.first, XMMatrixRotationQuaternion(q));
		}
	}*/
	VMDBoneChildRotation(currentBoneMatrix[0], 0);
	++frame %= 121;
	(this->*endCheck)();
}

void VMDPlayer::StopUpdate()
{

}

void VMDPlayer::VMDBoneRotation(const std::string& boneName, XMMATRIX& boneRotaMatrix)
{
	PMDBoneData* data = nullptr;
	unsigned int i = 0;
	for (; i < boneDatas.size(); i++)
	{
		if (boneDatas[i].boneName == boneName)
		{
			data = &boneDatas[i];
			break;
		}
	}
	if (data == nullptr) return;


	XMVECTOR offsetvec = XMLoadFloat3(&data->pos);
	XMMATRIX boneMat = DirectX::XMMatrixTranslationFromVector(-offsetvec);
	boneMat *= boneRotaMatrix;
	boneMat *= DirectX::XMMatrixTranslationFromVector(offsetvec);
	currentBoneMatrix[i] = boneMat * currentBoneMatrix[i];
}

void VMDPlayer::VMDBoneChildRotation(XMMATRIX& parentBoneMatrix, int parentIndex)
{
	for (auto& childIndex : boneNode.node[parentIndex])
	{
		currentBoneMatrix[childIndex] *= parentBoneMatrix;
		VMDBoneChildRotation(currentBoneMatrix[childIndex], childIndex);
	}
}

void VMDPlayer::Play(bool flag)
{
	loopFlag = flag;
	update = &VMDPlayer::PlayingUpdate;
	id = AnimationPlayerManager::Instance()->SetAnimation(this);
	if (loopFlag)
	{
		endCheck = &VMDPlayer::NonCheck;
	}
	else
	{
		endCheck = &VMDPlayer::LoopEndCheck;
	}
}

void VMDPlayer::LoopEndCheck()
{
	if (frame == 0) Stop();
}

void VMDPlayer::NonCheck()
{
}

void VMDPlayer::Stop()
{
	update = &VMDPlayer::StopUpdate;
	AnimationPlayerManager::Instance()->RemoveAnimation(id);
	id = -1;
}

void VMDPlayer::WriteBoneMatrix(ConstantBufferObject* matrixBuffer)
{
	matrixBuffer->WriteBuffer(&currentBoneMatrix[0], static_cast<unsigned int>(currentBoneMatrix.size() * sizeof(currentBoneMatrix[0])));
}