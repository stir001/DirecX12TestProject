#include "CollisionDetector.h"
#include "ActionStructs.h"
#include "XMFloatOperators.h"
#include "PlayerSH.h"
#include "Enemy.h"
#include "Rect.h"

const float SCALE = 2.0f;

CollisionDetector::CollisionDetector()
{
}


CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::CheckCollision(std::shared_ptr<PlayerSH>& c1, std::shared_ptr<Enemy>& c2)
{
	DirectX::XMFLOAT3 centerSub = {};
	float halfWidthAdd = 0;
	float halfHeightAdd = 0;
	for (auto& c1r : c1->GetCurrentActionRects())
	{
		for (auto& c2r : c2->GetCurrentActionRects())
		{
			if ((c1r.type == eACTION_RECT_TYPE_ATTACK && c2r.type == eACTION_RECT_TYPE_DAMAGE))
			{
				if (RectCollision(c1r.rect, c1->GetPos(), c1->IsTurn(), c2r.rect, c2->GetPos(), c2->IsTurn()))
				{
					c2->OnDamage();
				}
			}

			if ((c2r.type == eACTION_RECT_TYPE_ATTACK && c1r.type == eACTION_RECT_TYPE_DAMAGE))
			{
				if(RectCollision(c1r.rect,c1->GetPos(),c1->IsTurn(),c2r.rect,c2->GetPos(),c2->IsTurn()))
				{
					c1->OnDamage();
				}
			}
		}
	}
}

bool CollisionDetector::RectCollision(const Rect& r1, const DirectX::XMFLOAT3& r1offset, const bool isR1Turn, const Rect& r2, const DirectX::XMFLOAT3& r2offset, const bool isR2Turn)
{
	DirectX::XMFLOAT3 r1BasePosition = { r1.GetCenter().x * (isR1Turn ? -1 : 1) * SCALE + r1offset.x ,r1.GetCenter().y * SCALE + r1offset.y ,r1.GetCenter().z * SCALE + r1offset.z };
	DirectX::XMFLOAT3 r2BasePosition = { r2.GetCenter().x * (isR2Turn ? -1 : 1) * SCALE + r2offset.x ,r2.GetCenter().y * SCALE + r2offset.y ,r2.GetCenter().z * SCALE + r2offset.z };
	DirectX::XMFLOAT3 centerSub = r1BasePosition - r2BasePosition;
	float halfWidthAdd = r1.GetWidth() * SCALE / 2.0f + r2.GetWidth() * SCALE / 2.0f;
	float halfHeightAdd = r1.GetHeight() * SCALE / 2.0f + r2.GetHeight() * SCALE / 2.0f;

	return (abs(centerSub.x) <= halfWidthAdd) && (abs(centerSub.y) <= halfHeightAdd);
}