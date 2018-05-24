#include "ColliisonDetector.h"
#include "ICharactor.h"
#include "ActionStructs.h"
#include "XMFloat3Operators.h"

ColliisonDetector::ColliisonDetector()
{
}


ColliisonDetector::~ColliisonDetector()
{
}

void ColliisonDetector::CheckCollision(std::shared_ptr<ICharactor>& c1, std::shared_ptr<ICharactor>& c2)
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
				centerSub = c1r.rect.GetCenter() - c2r.rect.GetCenter();
				halfWidthAdd = c1r.rect.GetWidth() / 2.0f + c2r.rect.GetWidth() / 2.0f;
				halfHeightAdd = c1r.rect.GetHeight() / 2.0f + c2r.rect.GetHeight() / 2.0f;
				if (abs(centerSub.x) <= halfWidthAdd && abs(centerSub.y) <= halfHeightAdd)
				{
					c2->OnDamage();
				}
			}

			if ((c2r.type == eACTION_RECT_TYPE_ATTACK && c1r.type == eACTION_RECT_TYPE_DAMAGE))
			{
				centerSub = c1r.rect.GetCenter() - c2r.rect.GetCenter();
				halfWidthAdd = c1r.rect.GetWidth() / 2.0f + c2r.rect.GetWidth() / 2.0f;
				halfHeightAdd = c1r.rect.GetHeight() / 2.0f + c2r.rect.GetHeight() / 2.0f;
				if (abs(centerSub.x) <= halfWidthAdd && abs(centerSub.y) <= halfHeightAdd)
				{
					c1->OnDamage();
				}
			}
		}
	}
}