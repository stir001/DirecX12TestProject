#include "ScreenRayCast.h"
#include <Dx12MSLib.h>

ScreenRayCast::ScreenRayCast()
{
}


ScreenRayCast::~ScreenRayCast()
{
}

ScreenRayCast::Hit ScreenRayCast::RayCastPlane(const DirectX::XMFLOAT2 & screenPos, const DirectX::XMFLOAT3 & origin, const DirectX::XMFLOAT3 & normal)
{
	auto size = Dx12Ctrl::Instance().GetWindowSize();
	auto camera = GetCamera();
	auto cameraPos = camera->GetPos();
	auto mouse = screenPos;
	DirectX::XMFLOAT4 normNearPos = { (mouse.x / size.x - 0.5f) * 2.0f, (mouse.y / size.y - 0.5f) * 2.0f * -1.0f, 0.0f, 1.0f };
	auto inverseViewProj = InverseXMFloat4x4(camera->GetProjection()) * InverseXMFloat4x4(camera->GetView());
	auto worldNearClipPos = normNearPos * inverseViewProj;
	worldNearClipPos /= worldNearClipPos.w;

	auto nearCliplength = GetLengthXMFloat3(ConvertXMFloat4ToXMFloat3(worldNearClipPos - cameraPos));

	auto ray = NormalizeXMFloat3(ConvertXMFloat4ToXMFloat3(worldNearClipPos - cameraPos));

	auto toOrigin = origin - cameraPos;
	auto normalLength = DotXMFloat3(toOrigin, normal);
	auto rayNormalScale = DotXMFloat3(normal, ray);
	auto hitray = normalLength / rayNormalScale * ray;
	auto hitPos =  hitray + cameraPos;

	auto length = GetLengthXMFloat3(hitPos - cameraPos);

	return { length, hitPos };
}
