#include "Texture.h"
#include "../AssetTypes/TextureAsset.h"

namespace ReeeEngine
{
	Texture::Texture(Graphics& graphics, TextureAsset* asset)
	{
		// Create texture resource settings from texture asset.
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = asset->GetTexHeight();
		textureDesc.Height = asset->GetTexHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Setup sub resource data.
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = asset->GetBufferPointer();
		subresourceData.SysMemPitch = asset->GetTexPitch();

		// Create texture 2D to add to the resource view.
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		HRESULT result = GetDevice(graphics)->CreateTexture2D(&textureDesc, &subresourceData, &pTexture);
		LOG_DX_ERROR(result);

		// Create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srcDesc = {};
		srcDesc.Format = textureDesc.Format;
		srcDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srcDesc.Texture2D.MostDetailedMip = 0;
		srcDesc.Texture2D.MipLevels = 1;
		
		// Create resource on pipeline.
		result = GetDevice(graphics)->CreateShaderResourceView(pTexture.Get(), &srcDesc, &texture);
		LOG_DX_ERROR(result);
	}

	void Texture::Add(Graphics& graphics) noexcept
	{
		// Add the texture to the graphics pipeline.
		GetContext(graphics)->PSSetShaderResources(0u, 1u, texture.GetAddressOf());
	}
}