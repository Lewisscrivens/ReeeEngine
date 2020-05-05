#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* Texture class to setup default texture options and binding to the rendering pipeline. */
	class Texture : public ContextData
	{
	public:

		/* Construct default texture. */
		Texture(Graphics& graphics, class TextureAsset* asset);

		/* Add default texture to the rendering pipeline. */
		void Add(Graphics& graphics) noexcept override;

	protected:

		// The current texture pointer.
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	};
}

