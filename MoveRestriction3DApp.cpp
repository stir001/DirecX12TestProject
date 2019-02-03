#include "MoveRestriction3DApp.h"
#include "RandomGenerator.h"
#include "PlayerCharacter.h"
#include "PlaneCollisionCheckerXZ.h"
#include "TriangleDef.h"
#include <Dx12MSLib.h>

const float POLYGON_LENGTH = 10.0f;

MoveRestriction3DApp::MoveRestriction3DApp()
{
}


MoveRestriction3DApp::~MoveRestriction3DApp()
{
}

void MoveRestriction3DApp::Initialize(HINSTANCE hInst)
{
#ifndef _DEBUG
	ShaderCompiler::Instance().SetShaderDirPath("Shader/");
#endif
	Dx12CtrlInit(hInst, "1601295_^“ç§ˆê˜Y");

	auto& camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	mPlayer = std::make_shared<PlayerCharacter>(camera);
	CreateGround();
}

void MoveRestriction3DApp::Run()
{
	
	auto camera = Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(0);
	DxInput input;
	{

		while (ProcessMessage()) {
			input.UpdateKeyState();
			mPlayer->Move(input);
			CalYPosition();
			mPlayer->Draw();
			mGround->Draw();
		}
	}

	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}

void MoveRestriction3DApp::CreateGround()
{
	RandomGenerator generator;

	float heightRange = 5.0f;

	unsigned int xVertNum = 20;
	unsigned int zVertNum = 20;
	unsigned int vertAmountNum = xVertNum * zVertNum;
	std::vector<float> heightData(vertAmountNum);
	float pivotHeight = 0.0f;
	for (unsigned int i = 0; i <static_cast<unsigned int>(heightData.size()); ++i)
	{
		heightData[i] = pivotHeight += generator.GetValue(-heightRange, heightRange);
	}

	PrimitiveVertex vert;
	vert.normal = { 0.0f, 1.0f, 0.0f ,1.0f};
	mGroundVerts.resize(vertAmountNum);
	float vertXSpace = POLYGON_LENGTH;
	float vertZSpace = POLYGON_LENGTH;
	for (unsigned int i = 0; i < zVertNum; ++i)
	{
		for (unsigned int j = 0; j < xVertNum; ++j)
		{
			unsigned int idx = i * zVertNum + j;
			vert.pos = { j * vertXSpace, heightData[idx], i * vertZSpace ,1.0f};
			mGroundVerts[idx] = vert;
			mGroundVerts[idx].uv = { 1.0f / xVertNum * j, 1.0f - 1.0f / zVertNum * i };
		}
	}


	unsigned int polygonNum = (xVertNum - 1) * (zVertNum - 1) * 2;
	
	mGroundIndices.resize(polygonNum * 3);
	unsigned int idx = 0;
	for (unsigned int i = 0; i < xVertNum - 1; ++i)
	{
		for (unsigned int j = 0; j < zVertNum - 1; ++j)
		{
			mGroundIndices[idx++] = i * xVertNum + j;
			mGroundIndices[idx++] = i * xVertNum + j + 1 + xVertNum;
			mGroundIndices[idx++] = i * xVertNum + j + 1;

			mGroundIndices[idx++] = i * xVertNum + j;
			mGroundIndices[idx++] = i * xVertNum + j + xVertNum;
			mGroundIndices[idx++] = i * xVertNum + j + 1 + xVertNum;
		}
	}


	std::vector<std::vector<DirectX::XMFLOAT4>> normals(vertAmountNum);
	for (unsigned int i = 0; i < mGroundIndices.size() ;i += 3)
	{
		auto pos1 = ConvertXMFloat4ToXMFloat3(mGroundVerts[mGroundIndices[i]].pos);
		auto pos2 = ConvertXMFloat4ToXMFloat3(mGroundVerts[mGroundIndices[i + 1]].pos);
		auto pos3 = ConvertXMFloat4ToXMFloat3(mGroundVerts[mGroundIndices[i + 2]].pos);

		auto cross = ConvertXMFloat3ToXMFloat4(NormalizeXMFloat3(CrossXMFloat3(pos2 - pos1, pos3 - pos1)));

		normals[mGroundIndices[i]].push_back(cross);
		normals[mGroundIndices[i + 1]].push_back(cross);
		normals[mGroundIndices[i + 2]].push_back(cross);
	}

	for (unsigned int i = 0; i < vertAmountNum;++i)
	{
		DirectX::XMFLOAT4 norm = {};
		for (auto& n : normals[i])
		{
			norm += n;
		}
		mGroundVerts[i].normal = ConvertXMFloat3ToXMFloat4(NormalizeXMFloat3(ConvertXMFloat4ToXMFloat3(norm / static_cast<float>(normals[i].size()))));
	}

	auto primitive = std::make_shared<PrimitiveObject>("Move3DMesh", mGroundVerts, mGroundIndices);

	mGround = PrimitiveCreator::Instance().CreateCustumPrimitve(primitive);
	mGround->SetColor({ 0.2f,0.5f,0.0f,1.0f });
}

PRIMITIVE::Triangle MoveRestriction3DApp::GetPolygon(unsigned int polygonID) const
{
	PRIMITIVE::Triangle tri;
	tri.pos1 = ConvertXMFloat4ToXMFloat3(mGroundVerts[mGroundIndices[polygonID * 3]].pos);
	tri.pos2 = ConvertXMFloat4ToXMFloat3(mGroundVerts[mGroundIndices[polygonID * 3 + 1]].pos);
	tri.pos3 = ConvertXMFloat4ToXMFloat3(mGroundVerts[mGroundIndices[polygonID * 3 + 2]].pos);
	return tri;
}

void MoveRestriction3DApp::CalYPosition()
{
	for (int polyID = 0; polyID < mGroundIndices.size() / 3; ++polyID)
	{
		auto tri = GetPolygon(polyID);
		auto pos = mPlayer->GetPos();
		if (mChecker->IsInTriangle(pos, tri))
		{
			auto p1p2 = (tri.pos2 - tri.pos1);
			auto p1p3 = (tri.pos3 - tri.pos1);
			auto p2p3 = (tri.pos3 - tri.pos2);

			auto pivot = p1p2;
			bool isP1Pivot = false , isP2Pivot = false , isP3Pivot = false;

			if (p1p2.x == 0.0f)
			{
				isP1Pivot = true;
				isP2Pivot = true;
				isP3Pivot = false;

			}
			else if (p1p3.x == 0.0f)
			{
				isP1Pivot = true;
				isP2Pivot = false;
				isP3Pivot = true;

			}
			else if (p2p3.x == 0.0f)
			{
				isP1Pivot = false;
				isP2Pivot = true;
				isP3Pivot = true;

			}
			
			float zGrad = 0.0f;
			float xGrad = 0.0f;
			if (p1p2.z == 0.0f)
			{
				if (isP1Pivot == true)
				{
					pivot = tri.pos1;
					zGrad = (tri.pos3 - tri.pos1).y / (tri.pos3 - tri.pos1).z;
					xGrad = (tri.pos2 - tri.pos1).y / (tri.pos2 - tri.pos1).x;

				}
				else
				{
					pivot = tri.pos2;
					zGrad = (tri.pos3 - tri.pos2).y / (tri.pos3 - tri.pos2).z;
					xGrad = (tri.pos2 - tri.pos2).y / (tri.pos2 - tri.pos2).x;
				}
			}
			else if (p1p3.z == 0.0f)
			{
				if (isP1Pivot == true)
				{
					pivot = tri.pos1;

					zGrad = (tri.pos2 - tri.pos1).y / (tri.pos2 - tri.pos1).z;
					xGrad = (tri.pos3 - tri.pos1).y / (tri.pos3 - tri.pos1).x;


				}
				else
				{
					pivot = tri.pos3;
					
					zGrad = (tri.pos2 - tri.pos3).y / (tri.pos2 - tri.pos3).z;
					xGrad = (tri.pos1 - tri.pos3).y / (tri.pos1 - tri.pos3).x;
					
				}

			}
			else if (p2p3.z == 0.0f)
			{
				if (isP2Pivot == true)
				{
					pivot = tri.pos2;
					
					zGrad = (tri.pos1 - tri.pos2).y / (tri.pos1 - tri.pos2).z;
					xGrad = (tri.pos3 - tri.pos2).y / (tri.pos3 - tri.pos2).x;
					
				}
				else
				{
					pivot = tri.pos3;

					zGrad = (tri.pos1 - tri.pos3).y / (tri.pos1 - tri.pos3).z;
					xGrad = (tri.pos2 - tri.pos3).y / (tri.pos2 - tri.pos3).x;
					
				}
			}

			auto sub = pos - pivot;
			float height = pivot.y + sub.x * xGrad  + sub.z * zGrad ;
			
			pos.y = height;

			mPlayer->SetPosition(pos);
			
		}
	}
}
