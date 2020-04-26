#include "VertexShader.h"

namespace ReeeEngine
{
	VertexShader::VertexShader(Graphics& graphics, const std::wstring& filePath)
	{
		// Load the file into a D3D blob to be loaded as a vertex shader into the D3D device.
		HRESULT result = D3DReadFileToBlob(filePath.c_str(), &blobBytecode);
		LOG_DX_ERROR(result);
		result = GetDevice(graphics)->CreateVertexShader(blobBytecode->GetBufferPointer(), blobBytecode->GetBufferSize(), nullptr, &vertexShader);
		LOG_DX_ERROR(result);
	}

	void VertexShader::Add(Graphics& graphics) noexcept
	{
		// Set contexts current vertex shader.
		GetContext(graphics)->VSSetShader(vertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return blobBytecode.Get();
	}
}