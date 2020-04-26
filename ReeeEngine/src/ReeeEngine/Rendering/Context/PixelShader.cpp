#include "PixelShader.h"

namespace ReeeEngine
{
	PixelShader::PixelShader(Graphics& graphics, const std::wstring& filePath)
	{
		// Create blob from pixel shader file.
		Microsoft::WRL::ComPtr<ID3DBlob> blob;
		HRESULT result = D3DReadFileToBlob(filePath.c_str(), &blob);
		LOG_DX_ERROR(result);

		// Create the pixel shader to be accessed on the device.
		result = GetDevice(graphics)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
		LOG_DX_ERROR(result);
	}

	void PixelShader::Add(Graphics& graphics) noexcept
	{
		// Set contexts current pixel shader.
		GetContext(graphics)->PSSetShader(pixelShader.Get(), nullptr, 0u);
	}
}