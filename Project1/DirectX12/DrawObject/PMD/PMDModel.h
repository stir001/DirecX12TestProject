#pragma once
#include "Model.h"
#include "PMDDataStructure.h"

class ConstantBufferObject;
class VMDMotion;
class VMDPlayer;
struct ID3D12DescriptorHeap;

class PMDModel : public Model
{
	friend class PMDLoader;
public:
	~PMDModel();

	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_CAMERA,
		eROOT_PARAMATER_INDEX_LIGHT,
		eROOT_PARAMATER_INDEX_BONE_MATRIX,
		eROOT_PARAMATER_INDEX_MATERIAL,
		eROOT_PARAMATER_INDEX_TEXTURE,
		eROOT_PARAMATER_INDEX_SHADOWMAP
	};

	std::string mPath;
	PMDHeader mHeader;
	std::vector<PMDVertex> mVertexes;
	std::vector<unsigned short> mIndices;
	std::vector<PMDMaterial> mMaterials;
	std::vector<PMDBoneData> mBoneDatas;
	PMDIKList mIkLists;
	PMDSkin mSkins;//表情
	PMDSkinDisp mSkinDisps;//表情枠表示用リスト
	PMDBoneFrameDisp mBoneFrameDisps;
	PMDBoneDisp mBoneDisps;
	PMDToonPath mToonpath;
	PMDRigdBody mRigidBody;
	PMDJoint mJoint;

	BoneTree mBoneNode;

	void SetMaterialBuffer();
private:
	PMDModel();
};

