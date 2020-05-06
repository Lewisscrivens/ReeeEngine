#include "TextureAsset.h"
#include "../Graphics.h"

namespace ReeeEngine
{
	bool TextureAsset::Load(const std::string& path)
	{
		// Load file using DirectXTex api.
		HRESULT result = DirectX::LoadFromWICFile(std::wstring(path.begin(), path.end()).c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
		LOG_DX_ERROR(result);

		// Convert to the correct format.
		if (image.GetImage(0, 0, 0)->format != DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			DirectX::ScratchImage convertedImage;
			result = DirectX::Convert(*image.GetImage(0, 0, 0), DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
				DirectX::TEX_FILTER_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, convertedImage);
			LOG_DX_ERROR(result);
		}
		// Return true if load was successful.
		return true;
	}

	unsigned int TextureAsset::GetTexWidth() const
	{
		return (unsigned int)image.GetMetadata().width;
	}

	unsigned int TextureAsset::GetTexHeight() const
	{
		return (unsigned int)image.GetMetadata().height;
	}

	unsigned int TextureAsset::GetTexPitch() const
	{
		return (unsigned int)image.GetImage(0, 0, 0)->rowPitch;
	}

	uint8_t* TextureAsset::GetBufferPointer() const
	{
		return image.GetPixels();
	}
}