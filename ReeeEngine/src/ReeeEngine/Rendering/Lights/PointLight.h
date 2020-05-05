#pragma once
#include "../../Globals.h"
#include "../Context/ConstantBuffer.h"
#include "../../Math/Vector3D.h"

namespace ReeeEngine
{
	/* Point light class for creating a world dynamic light. */
	class PointLight
	{
	public:

		/* Point light constructor. */
		PointLight(Graphics& graphics);

		/* Light variable functions. */
		void SetPosition(const Vector3D& newPosition) noexcept;
		void SetAmbientColor(const Vector3D& newColor) noexcept;
		void SetDiffuseColor(const Vector3D& newColor) noexcept;
		void SetIntensity(const float newIntensity) noexcept;
		void SetAttenuation(const float newAttConst, const float newAttLin, const float newAttQuad) noexcept;

		/* Add light data to the rendering pipeline using the constant buffer. */
		void Add(Graphics& graphics, const DirectX::XMMATRIX& matrix) const noexcept;

	private:

		/* Private structure definition for point light constant buffer for PhongShader... */
		struct PointLightShaderSettings
		{
			alignas(16) DirectX::XMFLOAT3 pos;
			alignas(16) DirectX::XMFLOAT3 ambientColor;
			alignas(16) DirectX::XMFLOAT3 diffuseColor;
			float diffuseIntensity;
			float attConst;
			float attLin;
			float attQuad;
		};

	private:

		// Settings for point light.
		PointLightShaderSettings pointLightSetting;
		mutable PixelConstantBuffer<PointLightShaderSettings> constantBuffer;
	};
}
