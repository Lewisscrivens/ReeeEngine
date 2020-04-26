#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* New vertex data class wrapper for adding new vertex data and binding it to the rendering pipeline. */
	class VertextData : public ContextData
	{
	public:

		/* Create new vertex buffer on the graphics device. */
		template<class V>
		VertextData(Graphics& graphics, const std::vector<V>& vertices) : stride(sizeof(V))
		{
			// Create vertex buffer and add it to the rendering device.
			D3D11_BUFFER_DESC newBuffer = {};
			newBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			newBuffer.Usage = D3D11_USAGE_DEFAULT;
			newBuffer.CPUAccessFlags = 0u;
			newBuffer.MiscFlags = 0u;
			newBuffer.ByteWidth = UINT(sizeof(V) * vertices.size());
			newBuffer.StructureByteStride = sizeof(V);
			D3D11_SUBRESOURCE_DATA resourceData = {};
			resourceData.pSysMem = vertices.data();

			// Add buffer to graphics device and check for any errors/exceptions.
			HRESULT result = GetDevice(graphics)->CreateBuffer(&newBuffer, &resourceData, &vertexBuffer);
			LOG_DX_ERROR(result);
		}

		/* Add the buffer to the context. */
		virtual void Add(Graphics& graphics) noexcept override;

	protected:

		UINT stride;// Spacing of elements in the buffer.
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;// Pointer to new vertex buffer.
	};
}

