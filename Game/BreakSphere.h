#pragma once
#include "Dx12MSLib.h"

class BreakSphere : public PrimitiveObject
{
public:
	BreakSphere();
	~BreakSphere();
private:
	struct CutVertData
	{
		std::vector<PrimitiveVertex> minusVerts;
		std::vector<PrimitiveVertex> makedMinusVerts;
		std::vector<int> minusVertsIndices;
		std::vector<PrimitiveVertex> plusVerts;
		std::vector<PrimitiveVertex> makedPlusVerts;
		std::vector<int> plusVertsIndices;
		std::vector<PrimitiveVertex> originalVerts;
		void AddMinus(int vertNo);
		void AddPlus(int vertNo);
		void AddMisuTriangle(int vertNo1, int vertNo2, int vertNo3);
		bool IsInMinusVert(int vertNo) const;
		bool IsInPlusVert(int vertNo) const;
	};

	struct CutFace
	{
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 origin;
	};

	float DotNormal(const DirectX::XMFLOAT3& normal, const PrimitiveVertex& verts, const DirectX::XMFLOAT3& origin);
	int Sign(const float val);
	void CutTriangle(CutVertData& vertData,int vertNo, int vertVal1, int vertVal2, int vertVal3, const CutFace& face);

	
};

