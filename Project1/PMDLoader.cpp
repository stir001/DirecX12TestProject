#include "PMDLoader.h"
#include "File.h"
#include "PMDModel.h"
#include "IndexBufferObject.h"
#include "VertexBufferObject.h"
#include "ConstantBufferObject.h"
#include "RootSignatureObject.h"
#include "TextureObj.h"
#include "Dx12Ctrl.h"
#include "VMDPlayer.h"
#include "PMDController.h"

#include <d3d12.h>
#include <algorithm>

PMDLoader::PMDLoader()
{
}

PMDLoader::~PMDLoader()
{
}

PMDController* PMDLoader::Load(std::string path)
{

	fp = new File(path);
	GetRelativePath(path);
	loadingmodel = new PMDModel();
	controller = new PMDController();
	controller->model = loadingmodel;

	loadingmodel->path = path;
	models.push_back(loadingmodel);
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

	fp->Close();

	CreateIndexBuffer();
	CreateVertexBuffer();
	CreateTexture();
	CreateMaterialBuffer();
	CreateBoneMatrixBuffer();

	loadingmodel = nullptr;
	return controller;
}

void PMDLoader::LoadHeader()
{
	fp->LoadFile(&loadingmodel->header.fileType[0], 3);
	fp->LoadFile(&loadingmodel->header.version);
	fp->LoadFile(&loadingmodel->header.name[0],sizeof(loadingmodel->header.name));
	fp->LoadFile(&loadingmodel->header.comment[0], sizeof(loadingmodel->header.comment));
	fp->LoadFile(&loadingmodel->header.vertexcount);
}

void PMDLoader::LoadVertex()
{
	loadingmodel->vertexes.resize(loadingmodel->header.vertexcount);
	for (auto& v : loadingmodel->vertexes)
	{
		fp->LoadFile(&v.pos);
		fp->LoadFile(&v.normal);
		fp->LoadFile(&v.uv.x);
		fp->LoadFile(&v.uv.y);
		fp->LoadFile(&v.bornNum[0], 2);
		fp->LoadFile(&v.bornWeight);
		fp->LoadFile(&v.edgeFlag);
	}
}

void PMDLoader::LoadIndex()
{
	unsigned int indexcount = 0;
	fp->LoadFile(&indexcount);
	loadingmodel->indices.resize(indexcount);
	fp->LoadFile(&loadingmodel->indices[0], static_cast<unsigned int>(loadingmodel->indices.size()));
}

void PMDLoader::LoadMaterial()
{
	exittexcount = 0;
	unsigned int materialcount = 0;
	fp->LoadFile(&materialcount);
	loadingmodel->materials.resize(materialcount);
	std::vector<char*> texpaths;
	for (auto& m : loadingmodel->materials)
	{
		fp->LoadFile(&m.diffuse);
		fp->LoadFile(&m.alpha);
		fp->LoadFile(&m.specularity);
		fp->LoadFile(&m.specular);
		fp->LoadFile(&m.ambient);
		fp->LoadFile(&m.toonIndex);
		fp->LoadFile(&m.edgeFlag);
		fp->LoadFile(&m.indexCount);
		fp->LoadFile(&m.texturePath[0], sizeof(m.texturePath));
		if (m.texturePath[0] != 0)
		{
			exittexcount++;
			auto itr = std::find_if(texpaths.begin(), texpaths.end(), [&](char* tpath) {return strcmp(tpath , m.texturePath) == 0; });
			if (itr == texpaths.end())
			{
				m.texid = loadingmodel->texturecount;
				texpaths.push_back(m.texturePath);
				loadingmodel->texturecount++;
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
	fp->LoadFile(&bonecount);
	loadingmodel->boneDatas.resize(bonecount);
	loadingmodel->boneNode.node.resize(bonecount);
	int index = 0;
	for (auto& b : loadingmodel->boneDatas)
	{
		fp->LoadFile(&b.boneName[0], 20);
		fp->LoadFile(&b.parentIndex);
		fp->LoadFile(&b.tailIndex);
		fp->LoadFile(&b.type);
		fp->LoadFile(&b.ikParentIndex);
		fp->LoadFile(&b.pos);
		if (b.tailIndex != 0 && b.parentIndex != 0xffff)
		{
			loadingmodel->boneNode.node[b.parentIndex].push_back(index);
		}
		index++;
	}
}

void PMDLoader::LoadIkList()
{
	fp->LoadFile(&loadingmodel->ikLists.ikCount);
	loadingmodel->ikLists.ikDatas.resize(loadingmodel->ikLists.ikCount);
	for (auto& ik : loadingmodel->ikLists.ikDatas)
	{
		fp->LoadFile(&ik.ikBoneIndex);
		fp->LoadFile(&ik.ikTargetBoneIndex);
		fp->LoadFile(&ik.ikChainLength);
		fp->LoadFile(&ik.iterations);
		fp->LoadFile(&ik.controlWeight);
		ik.ikChildBoneIndex.resize(ik.ikChainLength);
		fp->LoadFile(&ik.ikChildBoneIndex[0], ik.ikChainLength);
	}
}

void PMDLoader::LoadSkin()
{
	fp->LoadFile(&loadingmodel->skins.skincount);
	loadingmodel->skins.datas.resize(loadingmodel->skins.skincount);
	for (auto& s : loadingmodel->skins.datas)
	{
		fp->LoadFile(&s.skinName[0], sizeof(s.skinName));
		fp->LoadFile(&s.skinvertexcount);
		fp->LoadFile(&s.skintype);
		s.vertexeis.resize(s.skinvertexcount);
		fp->LoadFile(&s.vertexeis[0], s.skinvertexcount);
	}
}

void PMDLoader::LoadSkinDisp()
{
	fp->LoadFile(&loadingmodel->skinDisps.skindispcount);
	loadingmodel->skinDisps.skinIndices.resize(loadingmodel->skinDisps.skindispcount);
	fp->LoadFile(&loadingmodel->skinDisps.skinIndices[0], loadingmodel->skinDisps.skindispcount);
}

void PMDLoader::LoadBoneFrameDisp()
{
	fp->LoadFile(&loadingmodel->boneFrameDisps.boneDispNameCount);
	unsigned char count = loadingmodel->boneFrameDisps.boneDispNameCount;
	loadingmodel->boneFrameDisps.names.resize(count);
	for (auto& n : loadingmodel->boneFrameDisps.names)
	{
		fp->LoadFile(&n.name[0], sizeof(n.name));
	}
}

void PMDLoader::LoadBoneDisp()
{
	fp->LoadFile(&loadingmodel->boneDisps.boneocunt);
	loadingmodel->boneDisps.boneDispIndices.resize(loadingmodel->boneDisps.boneocunt);
	for (auto& i : loadingmodel->boneDisps.boneDispIndices) {
		fp->LoadFile(&i.boneIndex);
		fp->LoadFile(&i.boneDispFrameIndex);
	}
}

void PMDLoader::SeekEngligh()
{
	//‰pŒêƒwƒbƒ_
	fp->SeekFile(sizeof(char));
	fp->SeekFile(sizeof(char) * 20);
	fp->SeekFile(sizeof(char) * 256);
	//‰pŒêƒ{[ƒ“–¼
	fp->SeekFile(static_cast<int>(sizeof(char) * 20 * loadingmodel->boneDatas.size()));
	//‰pŒêƒXƒLƒ“ƒŠƒXƒg
	fp->SeekFile(sizeof(char) * 20 * (loadingmodel->skins.skincount - 1));
	//‰pŒêƒ{[ƒ“˜g–¼•\Ž¦ƒŠƒXƒg
	fp->SeekFile(sizeof(char) * 50 * (loadingmodel->boneFrameDisps.boneDispNameCount));
}

void PMDLoader::LoadToonPath()
{
	fp->LoadFile(&loadingmodel->toonpath.path[0][0], 100 * 10);
}

void PMDLoader::LoadRigidBody()
{
	fp->LoadFile(&loadingmodel->rigidBody.rigidbodycount);
	loadingmodel->rigidBody.datas.resize(loadingmodel->rigidBody.rigidbodycount);
	for (auto& r : loadingmodel->rigidBody.datas)
	{
		fp->LoadFile(&r.rigidBodyName[0], sizeof(r.rigidBodyName));
		fp->LoadFile(&r.rigidbodyRelBoneIndex);
		fp->LoadFile(&r.rigidbodyGroupIndex);
		fp->LoadFile(&r.rigidbodyGroupTarget);
		fp->LoadFile(&r.shapeType);
		fp->LoadFile(&r.shapeW);
		fp->LoadFile(&r.shapeH);
		fp->LoadFile(&r.shapeD);
		fp->LoadFile(&r.pos);
		fp->LoadFile(&r.rot);
		fp->LoadFile(&r.weight);
		fp->LoadFile(&r.posDim);
		fp->LoadFile(&r.rotDim);
		fp->LoadFile(&r.recoil);
		fp->LoadFile(&r.friction);
		fp->LoadFile(&r.rigidbodyType);
	}
}

void PMDLoader::LoadJoint()
{
	fp->LoadFile(&loadingmodel->joint.jointcount);
	loadingmodel->joint.datas.resize(loadingmodel->joint.jointcount);
	for (auto& j : loadingmodel->joint.datas)
	{
		fp->LoadFile(&j.jointName[0],sizeof(j.jointName));
		fp->LoadFile(&j.jointRigidbodyA);
		fp->LoadFile(&j.jointRigidbodyB);
		fp->LoadFile(&j.jointPos);
		fp->LoadFile(&j.jointRot);
		fp->LoadFile(&j.constrainPos1);
		fp->LoadFile(&j.constrainPos2);
		fp->LoadFile(&j.constrainRot1);
		fp->LoadFile(&j.constrainRot2);
		fp->LoadFile(&j.springPos);
		fp->LoadFile(&j.springRot);
	}
}

void PMDLoader::CreateIndexBuffer()
{
	loadingmodel->indexBuffer = new IndexBufferObject(sizeof(loadingmodel->indices[0]),static_cast<unsigned int>(loadingmodel->indices.size()));
	loadingmodel->indexBuffer->WriteBuffer(&loadingmodel->indices[0], static_cast<unsigned int>(sizeof(loadingmodel->indices[0]) * loadingmodel->indices.size()));
}

void PMDLoader::CreateVertexBuffer()
{
	loadingmodel->vertexBuffer = new VertexBufferObject(sizeof(loadingmodel->vertexes[0]), static_cast<unsigned int>(loadingmodel->vertexes.size()));
	loadingmodel->vertexBuffer->WriteBuffer(&loadingmodel->vertexes[0], static_cast<unsigned int>(sizeof(loadingmodel->vertexes[0]) * loadingmodel->vertexes.size()));
}

void PMDLoader::CreateTexture()
{
	DX12CTRL_INSTANCE
	loadingmodel->textureObjects.resize(exittexcount);
	loadingmodel->textureDescHeap = d12.CreateTextureDescHeap(loadingmodel->path, exittexcount);
	UINT descsize = d12.GetDev()->GetDescriptorHandleIncrementSize(loadingmodel->textureDescHeap->GetDesc().Type);
	for (unsigned int i = 0;i < loadingmodel->materials.size();i++)
	{
		if (loadingmodel->materials[i].texid == -1) continue;
		std::wstring path;
		path.resize(relativePath.length() + 20);
		for (unsigned int i = 0; i < relativePath.length(); i++)
		{
			path[i] = relativePath[i];
		}
		size_t num;
		mbstowcs_s(&num, &path[path.length() - 20], 20, &loadingmodel->materials[i].texturePath[0], 20);
		D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle = loadingmodel->textureDescHeap->GetCPUDescriptorHandleForHeapStart();
		cpuhandle.ptr += loadingmodel->materials[i].texid * descsize;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle = loadingmodel->textureDescHeap->GetGPUDescriptorHandleForHeapStart();
		gpuhandle.ptr += loadingmodel->materials[i].texid * descsize;
		loadingmodel->textureObjects[loadingmodel->materials[i].texid] = d12.LoadTexture(path, cpuhandle, gpuhandle);
	}
}

void PMDLoader::CreateMaterialBuffer()
{
	loadingmodel->materialBuffer = new ConstantBufferObject(sizeof(Dx12Material), static_cast<unsigned int>(loadingmodel->materials.size()));
	loadingmodel->d12mat = new Dx12Material[loadingmodel->materials.size()];
	for (unsigned int i = 0; i < loadingmodel->materials.size(); i++)
	{
		loadingmodel->d12mat[i].alpha = loadingmodel->materials[i].alpha;
		loadingmodel->d12mat[i].diffuse = loadingmodel->materials[i].diffuse;
		loadingmodel->d12mat[i].ambient = loadingmodel->materials[i].ambient;
		loadingmodel->d12mat[i].specular = loadingmodel->materials[i].specular;
		loadingmodel->d12mat[i].specularity = loadingmodel->materials[i].specularity;
		loadingmodel->d12mat[i].offset = DirectX::XMFLOAT3(0.5, 0.5, 0);
	}

	loadingmodel->materialBuffer->WriteBuffer256Alignment(loadingmodel->d12mat, sizeof(Dx12Material), static_cast<unsigned int>(loadingmodel->materials.size()));
}

void PMDLoader::CreateBoneMatrixBuffer()
{
	controller->boneMatrixBuffer = new ConstantBufferObject(static_cast<unsigned int>(sizeof(DirectX::XMMATRIX) * loadingmodel->boneDatas.size()), 1);
	controller->boneMatrix.resize(loadingmodel->boneDatas.size());
	for (auto& bm : controller->boneMatrix) bm = DirectX::XMMatrixIdentity();
	controller->vmdPlayer = new VMDPlayer(loadingmodel->boneDatas, loadingmodel->boneNode, controller->boneMatrix);
}