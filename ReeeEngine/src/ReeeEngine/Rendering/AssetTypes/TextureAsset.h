#pragma once
#include "../../Globals.h"
#include "dxtex/DirectXTex.h"

namespace ReeeEngine
{
	/* Texture asset wrapper class to pass to the rendering pipeline for binding to the context. */
	class REEE_API TextureAsset
	{
	public:

		/* Default constructor. */
		TextureAsset() = default;
		~TextureAsset() = default;

		/* Load texture from file. */
		bool Load(const std::string& path);

		/* Texture information getters for the texture context data to use. */
		unsigned int GetTexWidth() const;
		unsigned int GetTexHeight() const;
		unsigned int GetTexPitch() const;
		uint8_t* GetBufferPointer() const;

	protected:

		// Image loaded.
		DirectX::ScratchImage image;
	};
}