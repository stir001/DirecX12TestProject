#include "PMDLoader.h"
#include "File.h"
#include "PMDModel.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"
#include "RootSignatureObject.h"
#include "TextureObject.h"
#include "Dx12Ctrl.h"
#include "VMDPlayer.h"
#include "PMDController.h"
#include "TextureLoader.h"
#include "PipelineStateObject.h"

#include <d3d12.h>
#include <algorithm>

const std::string PMDSHADER_PATH = "shader.hlsl";

PMDLoader::PMDLoader()
{
}

PMDLoader::~PMDLoader()
{
}

std::shared_ptr<PMDController> PMDLoader::Load(std::string path)
{
	if (mModels.find(path))
	{
		return std::shared_ptr<PMDController>();
	}
	mFp = new File(path);
	GetRelativePath(path);
	mLoadingmodel = new PMDModel();
	mController = new PMDController();
	mController->mModel = mLoadingmodel;

	mLoadingmodel->mPath = path;
	mModels.push_back(mLoadingmodel);
	LoadHeader();
	LoadVertex();
	LoadIndex();
	LoadMaterial();
	LoadBone();
	LoadIkList();
	LoadSkin();
	LoadSkinDisp();
	LoadBoneFrameDisp();
	LoadBoneDisp();
	SeekEngligh();
	LoadToonPath();
	LoadRigidBody();
	LoadJoint();

	mFp->Close();

	CreateIndexBuffer();
	CreateVertexBuffer();
	CreateTexture();
	CreateMaterialBuffer();
	CreateBoneMatrixBuffer();

	mLoadingmodel = nullptr;
	return mController;
}

void PMDLoader::LoadHeader()
{
	mFp->LoadFile(&mLoadingmodel->mHeader.fileType[0], 3);
	mFp->LoadFile(&mLoadingmodel->mHeader.version);
	mFp->LoadFile(&mLoadingmodel->mHeader.name[0],sizeof(mLoadingmodel->mHeader.name));
	mFp->LoadFile(&mLoadingmodel->mHeader.comment[0], sizeof(mLoadingmodel->mHeader.comment));
	mFp->LoadFile(&mLoadingmodel->mHeader.vertexcount);
}

void PMDLoader::LoadVertex()
{
	mLoadingmodel->mVertexes.resize(mLoadingmodel->mHeader.vertexcount);
	for (auto& v : mLoadingmodel->mVertexes)
	{
		mFp->LoadFile(&v.pos);
		mFp->LoadFile(&v.normal);
		mFp->LoadFile(&v.uv.x);
		mFp->LoadFile(&v.uv.y);
		mFp->LoadFile(&v.bornNum[0], 2);
		mFp->LoadFile(&v.bornWeight);
		mFp->LoadFile(&v.edgeFlag);
	}
}

void PMDLoader::LoadIndex()
{
	unsigned int indexcount = 0;
	mFp->LoadFile(&indexcount);
	mLoadingmodel->mIndices.resize(indexcount);
	mFp->LoadFile(&mLoadingmodel->mIndices[0], static_cast<unsigned int>(mLoadingmodel->mIndices.size()));
}

void PMDLoader::LoadMaterial()
{
	exittexcount = 0;
	unsigned int materialcount = 0;
	mFp->LoadFile(&materialcount);
	mLoadingmodel->mMaterials.resize(materialcount);
	std::vector<char*> texpaths;
	for (auto& m : mLoadingmodel->mMaterials)
	{
		mFp->LoadFile(&m.diffuse);
		mFp->LoadFile(&m.alpha);
		mFp->LoadFile(&m.specularity);
		mFp->LoadFile(&m.specular);
		mFp->LoadFile(&m.ambient);
		mFp->LoadFile(&m.toonIndex);
		mFp->LoadFile(&m.edgeFlag);
		mFp->LoadFile(&m.indexCount);
		mFp->LoadFile(&m.texturePath[0], sizeof(m.texturePath));
		if (m.texturePath[0] != 0)
		{
			exittexcount++;
			auto itr = std::find_if(texpaths.begin(), texpaths.end(), [&](char* tpath) {return strcmp(tpath , m.texturePath) == 0; });
			if (itr == texpaths.end())
			{
				m.texid = mLoadingmodel->mTexturecount;
				texpaths.push_back(m.texturePath);
				mLoadingmodel->mTexturecount++;
			}
			else
			{
				m.texid = static_cast<int>(itr - texpaths.begin());
			}
			
		}
		else m.texid = -1;
	}
}

void PMDLoader::LoadBone()
{
	unsigned short bonecount = 0;
	mFp->LoadFile(&bonecount);
	mLoadingmodel->mBoneDatas.resize(bonecount);
	mLoadingmodel->mBoneNode.node.resize(bonecount);
	int index = 0;
	for (auto& b : mLoadingmodel->mBoneDatas)
	{
		mFp->LoadFile(&b.boneName[0], 20);
		mFp->LoadFile(&b.parentIndex);
		mFp->LoadFile(&b.tailIndex);
		mFp->LoadFile(&b.type);
		mFp->LoadFile(&b.ikParentIndex);
		mFp->LoadFile(&b.pos);
		if (b.tailIndex != 0 && b.parentIndex != 0xffff)
		{
			mLoadingmodel->mBoneNode.node[b.parentIndex].push_back(index);
		}
		index++;
	}
}

void PMDLoader::LoadIkList()
{
	mFp->LoadFile(&mLoadingmodel->mIkLists.ikCount);
	mLoadingmodel->mIkLists.ikDatas.resize(mLoadingmodel->mIkLists.ikCount);
	for (auto& ik : mLoadingmodel->mIkLists.ikDatas)
	{
		mFp->LoadFile(&ik.ikBoneIndex);
		mFp->LoadFile(&ik.ikTargetBoneIndex);
		mFp->LoadFile(&ik.ikChainLength);
		mFp->LoadFile(&ik.iterations);
		mFp->LoadFile(&ik.controlWeight);
		ik.ikChildBoneIndex.resize(ik.ikChainLength);
		mFp->LoadFile(&ik.ikChildBoneIndex[0], ik.ikChainLength);
	}
}

void PMDLoader::LoadSkin()
{
	mFp->LoadFile(&mLoadingmodel->mSkins.skincount);
	mLoadingmodel->mSkins.datas.resize(mLoadingmodel->mSkins.skincount);
	for (auto& s : mLoadingmodel->mSkins.datas)
	{
		mFp->LoadFile(&s.skinName[0], sizeof(s.skinName));
		mFp->LoadFile(&s.skinvertexcount);
		mFp->LoadFile(&s.skintype);
		s.vertexeis.resize(s.skinvertexcount);
		mFp->LoadFile(&s.vertexeis[0], s.skinvertexcount);
	}
}

void PMDLoader::LoadSkinDisp()
{
	mFp->LoadFile(&mLoadingmodel->mSkinDisps.skindispcount);
	mLoadingmodel->mSkinDisps.skinIndices.resize(mLoadingmodel->mSkinDisps.skindispcount);
	mFp->LoadFile(&mLoadingmodel->mSkinDisps.skinIndices[0], mLoadingmodel->mSkinDisps.skindispcount);
}

void PMDLoader::LoadBoneFrameDisp()
{
	mFp->LoadFile(&mLoadingmodel->mBoneFrameDisps.boneDispNameCount);
	unsigned char count = mLoadingmodel->mBoneFrameDisps.boneDispNameCount;
	mLoadingmodel->mBoneFrameDisps.names.resize(count);
	for (auto& n : mLoadingmodel->mBoneFrameDisps.names)
	{
		mFp->LoadFile(&n.name[0], sizeof(n.name));
	}
}

void PMDLoader::LoadBoneDisp()
{
	mFp->LoadFile(&mLoadingmodel->mBoneDisps.boneocunt);
	mLoadingmodel->mBoneDisps.boneDispIndices.resize(mLoadingmodel->mBoneDisps.boneocunt);
	for (auto& i : mLoadingmodel->mBoneDisps.boneDispIndices) {
		mFp->LoadFile(&i.boneIndex);
		mFp->LoadFile(&i.boneDispFrameIndex);
	}
}

void PMDLoader::SeekEngligh()
{
	//英語ヘッダ
	mFp->SeekFile(sizeof(char));
	mFp->SeekFile(sizeof(char) * 20);
	mFp->SeekFile(sizeof(char) * 256);
	//英語ボーン名
	mFp->SeekFile(static_cast<int>(sizeof(char) * 20 * mLoadingmodel->mBoneDatas.size()));
	//英語スキンリスト
	mFp->SeekFile(sizeof(char) * 20 * (mLoadingmodel->mSkins.skincount - 1));
	//英語ボーン枠名表示リスト
	mFp->SeekFile(sizeof(char) * 50 * (mLoadingmodel->mBoneFrameDisps.boneDispNameCount));
}

void PMDLoader::LoadToonPath()
{
	mFp->LoadFile(&mLoadingmodel->mToonpath.path[0][0], 100 * 10);
}

void PMDLoader::LoadRigidBody()
{
	mFp->LoadFile(&mLoadingmodel->mRigidBody.rigidbodycount);
	mLoadingmodel->mRigidBody.datas.resize(mLoadingmodel->mRigidBody.rigidbodycount);
	for (auto& r : mLoadingmodel->mRigidBody.datas)
	{
		mFp->LoadFile(&r.rigidBodyName[0], sizeof(r.rigidBodyName));
		mFp->LoadFile(&r.rigidbodyRelBoneIndex);
		mFp->LoadFile(&r.rigidbodyGroupIndex);
		mFp->LoadFile(&r.rigidbodyGroupTarget);
		mFp->LoadFile(&r.shapeType);
		mFp->LoadFile(&r.shapeW);
		mFp->LoadFile(&r.shapeH);
		mFp->LoadFile(&r.shapeD);
		mFp->LoadFile(&r.pos);
		mFp->LoadFile(&r.rot);
		mFp->LoadFile(&r.weight);
		mFp->LoadFile(&r.posDim);
		mFp->LoadFile(&r.rotDim);
		mFp->LoadFile(&r.recoil);
		mFp->LoadFile(&r.friction);
		mFp->LoadFile(&r.rigidbodyType);
	}
}

void PMDLoader::LoadJoint()
{
	mFp->LoadFile(&mLoadingmodel->mJoint.jointcount);
	mLoadingmodel->mJoint.datas.resize(mLoadingmodel->mJoint.jointcount);
	for (auto& j : mLoadingmodel->mJoint.datas)
	{
		mFp->LoadFile(&j.jointName[0],sizeof(j.jointName));
		mFp->LoadFile(&j.jointRigidbodyA);
		mFp->LoadFile(&j.jointRigidbodyB);
		mFp->LoadFile(&j.jointPos);
		mFp->LoadFile(&j.jointRot);
		mFp->LoadFile(&j.constrainPos1);
		mFp->LoadFile(&j.constrainPos2);
		mFp->LoadFile(&j.constrainRot1);
		mFp->LoadFile(&j.constrainRot2);
		mFp->LoadFile(&j.springPos);
		mFp->LoadFile(&j.springRot);
	}
}

void PMDLoader::CreateIndexBuffer()
{
	mLoadingmodel->mIndexBuffer.reset(new IndexBufferObject("PMDIndexBuffer", Dx12Ctrl::Instance()->GetDev(), sizeof(mLoadingmodel->mIndices[0]),static_cast<unsigned int>(mLoadingmodel->mIndices.size())));
	mLoadingmodel->mIndexBuffer->WriteBuffer(&mLoadingmodel->mIndices[0], static_cast<unsigned int>(sizeof(mLoadingmodel->mIndices[0]) * mLoadingmodel->mIndices.size()));
}

void PMDLoader::CreateVertexBuffer()
{
	mLoadingmodel->mVertexBuffer.reset(new VertexBufferObject("PMDVertexBuffer", Dx12Ctrl::Instance()->GetDev(), sizeof(mLoadingmodel->mVertexes[0]), static_cast<unsigned int>(mLoadingmodel->mVertexes.size())));
	mLoadingmodel->mVertexBuffer->WriteBuffer(&mLoadingmodel->mVertexes[0], static_cast<unsigned int>(sizeof(mLoadingmodel->mVertexes[0]) * mLoadingmodel->mVertexes.size()));
}

void PMDLoader::CreateTexture()
{
	DX12CTRL_INSTANCE
	mLoadingmodel->mTextureObjectects.resize(exittexcount);
	//mLoadingmodel->mTextureDescHeap = TextureLoader::Instance()->CreateTexDescHeap(mLoadingmodel->mPath, exittexcount);
	//UINT descsize = d12->GetDev()->GetDescriptorHandleIncrementSize(mLoadingmodel->mTextureDescHeap->GetDesc().Type);
	for (unsigned int i = 0;i < mLoadingmodel->mMaterials.size();i++)
	{
		if (mLoadingmodel->mMaterials[i].texid == -1) continue;
		std::wstring mPath;
		mPath.resize(mRelativePath.length() + 20);
		for (unsigned int i = 0; i < mRelativePath.length(); i++)
		{
			mPath[i] = mRelativePath[i];
		}
		size_t num;
		mbstowcs_s(&num, &mPath[mPath.length() - 20], 20, &mLoadingmodel->mMaterials[i].texturePath[0], 20);
		//D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle = mLoadingmodel->mTextureDescHeap->GetCPUDescriptorHandleForHeapStart();
		//cpuhandle.ptr += mLoadingmodel->mMaterials[i].texid * descsize;
		//D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle = mLoadingmodel->mTextureDescHeap->GetGPUDescriptorHandleForHeapStart();
		//gpuhandle.ptr += mLoadingmodel->mMaterials[i].texid * descsize;
		//mLoadingmodel->mTextureObjectects[mLoadingmodel->mMaterials[i].texid] = TextureLoader::Instance()->LoadTexture(mPath, cpuhandle, gpuhandle);
	}
}

void PMDLoader::CreateMaterialBuffer()
{
	mLoadingmodel->mMaterialBuffer.reset(new ConstantBufferObject("PMDMaterialBuffer", Dx12Ctrl::Instance()->GetDev(),sizeof(Dx12Material), static_cast<unsigned int>(mLoadingmodel->mMaterials.size())));
	mLoadingmodel->mD12mat = new Dx12Material[mLoadingmodel->mMaterials.size()];
	for (unsigned int i = 0; i < mLoadingmodel->mMaterials.size(); i++)
	{
		mLoadingmodel->mD12mat[i].alpha = mLoadingmodel->mMaterials[i].alpha;
		mLoadingmodel->mD12mat[i].diffuse = mLoadingmodel->mMaterials[i].diffuse;
		mLoadingmodel->mD12mat[i].ambient = mLoadingmodel->mMaterials[i].ambient;
		mLoadingmodel->mD12mat[i].specular = mLoadingmodel->mMaterials[i].specular;
		mLoadingmodel->mD12mat[i].specularity = mLoadingmodel->mMaterials[i].specularity;
		mLoadingmodel->mD12mat[i].offset = DirectX::XMFLOAT3(0.5, 0.5, 0);
	}

	mLoadingmodel->mMaterialBuffer->WriteBuffer256Alignment(mLoadingmodel->mD12mat, sizeof(Dx12Material), static_cast<unsigned int>(mLoadingmodel->mMaterials.size()));
}

void PMDLoader::CreateBoneMatrixBuffer()
{
	mController->mBoneMatrixBuffer = new ConstantBufferObject("PMDBoneMatrixBuffer", Dx12Ctrl::Instance()->GetDev(),static_cast<unsigned int>(sizeof(DirectX::XMMATRIX) * mLoadingmodel->mBoneDatas.size()), 1);
	mController->mBoneMatrix.resize(mLoadingmodel->mBoneDatas.size());
	for (auto& bm : mController->mBoneMatrix) bm = DirectX::XMMatrixIdentity();
	mController->mVmdPlayer = new VMDPlayer(mLoadingmodel->mBoneDatas, mLoadingmodel->mBoneNode, mController->mBoneMatrix);
}

void PMDLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	D3D12_INPUT_ELEMENT_DESC inputDescs[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "BONENO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
	};

	CD3DX12_RASTERIZER_DESC rastarizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	rastarizer.CullMode = D3D12_CULL_MODE_NONE;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);	//ブレンドするか
	gpsDesc.DepthStencilState.DepthEnable = true;			//デプスを使うか
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = false;		//???
	gpsDesc.InputLayout.NumElements = sizeof(inputDescs) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	gpsDesc.InputLayout.pInputElementDescs = inputDescs;	//要素へのポインタ(先頭?)
	gpsDesc.pRootSignature = GetRootSignature().Get();				//ルートシグネチャポインタ
	gpsDesc.RasterizerState = rastarizer;	//ラスタライザーの設定
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleMask = 0xffffff;
	gpsDesc.NodeMask = 0;

	gpsDesc.VS = mShader.vertexShader;
	gpsDesc.PS = mShader.pixcelShader;
	gpsDesc.DS;
	gpsDesc.GS;
	gpsDesc.HS;

	mPipelinestate.reset(new PipelineStateObject(gpsDesc,dev));
}

void PMDLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
}
