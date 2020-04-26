#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* Class to make adding new pixel shaders more seamless on the rendering pipeline. */
	class PixelShader : public ContextData
	{
	public:

		/* Constructor for creating the pixel shader data to be later binded to the context object. */
		PixelShader(Graphics& graphics, const std::wstring& filePath);

		/* Function to add the created pixel shader to the rendering pipeline. */
		virtual void Add(Graphics& graphics) noexcept override;

	protected:

		// Pixel shader created pointer.
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	};
}
