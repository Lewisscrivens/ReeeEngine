#include "InputLayout.h"

namespace ReeeEngine 
{
	InputLayout::InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderBytecode)
	{
		// Create the new input layout and throw any exceptions returned from the error macro check.
		HRESULT result = GetDevice(graphics)->CreateInputLayout(layout.data(), (UINT)layout.size(), 
			vertexShaderBytecode->GetBufferPointer(), vertexShaderBytecode->GetBufferSize(), &inputLayout);
		LOG_DX_ERROR(result);
	}

	void InputLayout::Add(Graphics& graphics) noexcept
	{
		GetContext(graphics)->IASetInputLayout(inputLayout.Get());
	}
}