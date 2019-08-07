#pragma once
#include "Game\Scene\Scene.h"
#include <memory>
#include <list>
#include <DirectXMath.h>
#include <vector>

class GameObject;
class PrimitiveCutter;
struct PrimitiveVertex;

class FreeCutScene :
	public Scene
{
public:
	FreeCutScene(int type);
	~FreeCutScene();

	enum class PrimitiveType
	{
		CUBE,
		SPHERE,
		CAPSULE,
		CONE,
	};

	void Initialize();
	void Terminate();
	void Update(const DxInput& input);
	void Draw();

private:
	void MoveCamera(const DxInput & input);
	void WaitCutStart(const DxInput& input);
	void ChoseCutEnd(const DxInput& input);
	void Cut();
	DirectX::XMFLOAT3 RePlaceVertsPos(std::vector<PrimitiveVertex>& verts);
	bool IsHitSpherePlane(const DirectX::XMFLOAT3& sphOrigin, const float radius,
		const DirectX::XMFLOAT3& plOrigin, const DirectX::XMFLOAT3 normal);

	void (FreeCutScene::*mUpdateState)(const DxInput& input);
	std::list<std::shared_ptr<GameObject>> mObjects;
	std::shared_ptr<PrimitiveCutter> mCutter;
	DirectX::XMFLOAT3 mStart;
	DirectX::XMFLOAT3 mEnd;
	DirectX::XMFLOAT2 mMousePos;
};

