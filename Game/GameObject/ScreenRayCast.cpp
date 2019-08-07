#include "ScreenRayCast.h"
#include <Dx12MSLib.h>

ScreenRayCast::ScreenRayCast()
{
}


ScreenRayCast::~ScreenRayCast()
{
}

ScreenRayCast::Hit ScreenRayCast::RayCastPlane(const DirectX::XMFLOAT2& screenPos, const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& normal)
{
	auto ray = GetRay(screenPos);
	auto cameraPos = ConvertToXMFloat3(GetCamera()->GetPos());
	
	auto oDn = Dot(origin, normal);
	auto cDn = Dot(cameraPos, normal);
	auto rDn = Dot(ray, normal);
	auto hitray = -(cDn - oDn) / rDn * ray;

	auto hitPos =  hitray + cameraPos;

	auto zero = Dot(Normalize(hitPos - origin), normal);

	auto length = GetLength(hitPos - cameraPos);

	return { length, hitPos };
}

bool ScreenRayCast::RayCastTriangle(const DirectX::XMFLOAT2& screenPos, const DirectX::XMFLOAT3& pos0, const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
{
	auto v1 = pos1 - pos0;
	auto v2 = pos2 - pos1;
	auto v3 = pos0 - pos2;
	auto normal = Normalize(Cross(pos2 - pos0, v1));

	auto hit = RayCastPlane(screenPos, pos0, normal);

	auto hit1 = hit.hitPos - pos0;
	auto hit2 = hit.hitPos - pos1;
	auto hit3 = hit.hitPos - pos2;

	auto cross1 = Normalize(Cross(Normalize(hit1), Normalize(v1)));
	auto cross2 = Normalize(Cross(Normalize(hit2), Normalize(v2)));
	auto cross3 = Normalize(Cross(Normalize(hit3), Normalize(v3)));

	float epsilon = 1e-06f;

	if (cross1 == cross2 && cross2 == cross3)
	{
		return true;
	}

	return false;
}

DirectX::XMFLOAT3 ScreenRayCast::GetRay(const DirectX::XMFLOAT2& screenPos)
{
	auto farPos = GetScreenSpace(screenPos, 1.0f);
	auto cameraPos = GetCamera()->GetPos();
	auto ray = Normalize(farPos - cameraPos);
	return ray;
}

DirectX::XMFLOAT3 ScreenRayCast::GetScreenSpace(const DirectX::XMFLOAT2 & screenPos, float depth)
{
	if (depth > 1.0f)
	{
		depth = 1.0f;
	}
	else if (depth < 0.0f)
	{
		depth = 0.0f;
	}

	auto camera = GetCamera();
	auto cameraPos = camera->GetPos();
	auto mouse = screenPos;
	auto size = Dx12Ctrl::Instance().GetWindowSize();
	DirectX::XMFLOAT4 normNearPos = { (mouse.x / size.x - 0.5f) * 2.0f, (mouse.y / size.y - 0.5f) * 2.0f * -1.0f, depth, 1.0f };
	auto inverseViewProj = Inverse(camera->GetView() * camera->GetProjection());
	auto worldPos = normNearPos * inverseViewProj;
	worldPos /= worldPos.w;
	return ConvertToXMFloat3(worldPos);
}

bool ScreenRayCast::RayCastSphere(const DirectX::XMFLOAT2& screenPos, const DirectX::XMFLOAT3& origin, const float raidus)
{
	auto ray = GetRay(screenPos);
	auto cameraPos = GetCamera()->GetPos();

	auto vec =  origin - cameraPos;
	auto rayLength = Dot(vec, ray);

	auto hitRay = ray * rayLength;
	auto hitPos = cameraPos + hitRay;

	if (GetLength(origin - hitPos) < raidus)
	{
		return true;
	}

	return false;
}

bool ScreenRayCast::MollerIntersection(const DirectX::XMFLOAT2& screenPos, const DirectX::XMFLOAT3& pos0, const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
{
	const float eplison = 1.0e-07f;

	auto rayVector = GetRay(screenPos);
	auto rayOrigin = ConvertToXMFloat3(GetCamera()->GetPos());

	auto v0v1 = pos1 - pos0;
	auto v0v2 = pos2 - pos0;
	auto pvec = Normalize(Cross(rayVector, v0v2));
	float det = Dot(v0v1, pvec);

	float invDet = 1 / det;

	auto tvec = rayOrigin - pos0;
	auto u = Dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	auto qvec = Normalize(Cross(tvec, v0v1));
	auto v = Dot(rayVector, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	auto t = Dot(v0v2, qvec) * invDet;

	auto rayhit = rayOrigin + t * rayVector;
	auto point = pos0 + v0v1 * v + v0v2 * u;

	return true;
}

