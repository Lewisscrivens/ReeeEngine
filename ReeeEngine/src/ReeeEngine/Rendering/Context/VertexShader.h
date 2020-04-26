#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* Class to make adding new vertex shaders more seamless on the rendering pipeline. */
	class VertexShader : public ContextData
	{
	public:

		/* Constructor for creating the pixel shader data to be later binded to the context object. */
		VertexShader(Graphics& graphics, const std::wstring& filePath);

		/* Function to add the created vertex shader to the rendering pipeline. */
		virtual void Add(Graphics& gfx) noexcept override;

		/* Get the blob byte code from the current vertex shader loaded into this class. */
		ID3DBlob* GetBytecode() const noexcept;

	protected:

		// Declare pointers to the vertex shader created and its bytecode.
		Microsoft::WRL::ComPtr<ID3DBlob> blobBytecode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	};
}