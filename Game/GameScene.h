#pragma once
#include "Scene.h"
#include <memory>
#include <DirectXMath.h>
#include <vector>

class PrimitiveController;
class PrimitiveCutter;
class PrimitiveObject;
struct PrimitiveVertex;
class RandomGenerator;

class GameScene :
	public Scene
{
public:
	enum class PrimitiveType
	{
		CUBE,
		SPHERE,
		CAPSULE,
		CONE,
		PLANE
	};

	GameScene(const PrimitiveType type);
	~GameScene();
	void Initialize();
	void Terminate();
	void Update(const DxInput& input);
	void Draw();
	void Cut(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& normal);
private:
	std::shared_ptr<PrimitiveController> mBaseCtrl;
	std::shared_ptr<PrimitiveController> mPlusCtrl;
	std::shared_ptr<PrimitiveController> mMinusCtrl;
	std::shared_ptr<PrimitiveObject> mBase;
	std::shared_ptr<PrimitiveObject> mPlus;
	std::shared_ptr<PrimitiveObject> mMinus;
	std::shared_ptr<PrimitiveCutter> mCutter;
	std::shared_ptr<RandomGenerator> mRandom;

	void (GameScene::*mDrawState)();
	void (GameScene::*mCutState)();

	void DrawBase();
	void DrawPlus();
	void DrawMinus();
	void DrawCut();

	/**
	*	@brief	全ての頂点座標を平均値を原点、中心とした座標に再計算する
	*	@param[in,out]	再計算する頂点群
	*	@return			原点とした平均値
	*/
	DirectX::XMFLOAT3 RePlaceVertsPos(std::vector<PrimitiveVertex>& verts);
};