#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* Input layout class to act as a wrapper for creating new InputLayouts onto the current graphics device. */
	class InputLayout : public ContextData
	{
	public:

		/* Constructor to create all information needed for the new input layout class. */
		InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderBytecode);

		/* Function to bind the input layout to the rendering pipeline. */
		virtual void Add(Graphics& graphics) noexcept override;

	protected:

		// Pointer to the created input layout.
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	};
}