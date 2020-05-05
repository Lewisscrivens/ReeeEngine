#include "PointLight.h"
#include "../../Application.h"
#include "../../World/World.h"
#include "../../World/Components/CameraComponent.h"

namespace ReeeEngine
{
	PointLight::PointLight(Graphics& graphics) : constantBuffer(graphics) 
	{
		pointLightSetting.pos = { 0.0f, 0.0f, 0.0f };
		pointLightSetting.ambientColor = { 0.05f, 0.05f, 0.05f };
		pointLightSetting.diffuseColor = { 1.0f, 1.0f, 1.0f };
		pointLightSetting.diffuseIntensity = 5.0f;
		pointLightSetting.attConst = 1.0f;
		pointLightSetting.attLin = 0.05f;
		pointLightSetting.attQuad = 0.008f;
	}

	void PointLight::SetPosition(const Vector3D& newPosition) noexcept
	{
		pointLightSetting.pos = newPosition.ToFloat3();
	}

	void PointLight::SetAmbientColor(const Vector3D& newColor) noexcept
	{
		pointLightSetting.ambientColor = newColor.ToFloat3();
	}

	void PointLight::SetDiffuseColor(const Vector3D& newColor) noexcept
	{
		pointLightSetting.diffuseColor = newColor.ToFloat3();
	}

	void PointLight::SetIntensity(const float newIntensity) noexcept
	{
		pointLightSetting.diffuseIntensity = newIntensity;
	}

	void PointLight::SetAttenuation(const float newAttConst, const float newAttLin, const float newAttQuad) noexcept
	{
		pointLightSetting.attConst = newAttConst;
		pointLightSetting.attLin = newAttLin;
		pointLightSetting.attQuad = newAttQuad;
	}

	void PointLight::Add(Graphics& graphics, const DirectX::XMMATRIX& matrix) const noexcept
	{
		auto settings = pointLightSetting;
		const auto posVector = DirectX::XMLoadFloat3(&pointLightSetting.pos);
		DirectX::XMStoreFloat3(&settings.pos, DirectX::XMVector3Transform(posVector, matrix));
		constantBuffer.Update(graphics, settings);
		constantBuffer.Add(graphics);
	}
}