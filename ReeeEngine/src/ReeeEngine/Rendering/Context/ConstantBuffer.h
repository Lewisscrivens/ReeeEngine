#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* Default constant buffer class for binding a constant buffer to the rendering pipeline. */
	template<typename C>
	class ConstantBuffer : public ContextData
	{
	public:

		/* Update the constant buffer. */
		void Update(Graphics& graphics, const C& consts)
		{
			// Map the constant buffer to the context.
			D3D11_MAPPED_SUBRESOURCE msr;
			HRESULT result = GetContext(graphics)->Map(constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
			LOG_DX_ERROR(result);
			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(graphics)->Unmap(constantBuffer.Get(), 0u);
		}

		/* Constant buffer constructor to setup default buffer using C template. */
		ConstantBuffer(Graphics& graphics, const C& consts)
		{
			// Setup constant buffer settings.
			D3D11_BUFFER_DESC constantBufferSettings;
			constantBufferSettings.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferSettings.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferSettings.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferSettings.MiscFlags = 0u;
			constantBufferSettings.ByteWidth = sizeof(consts);
			constantBufferSettings.StructureByteStride = 0u;

			// Create resource data and the buffer in the D3D device...
			D3D11_SUBRESOURCE_DATA constantBufferResources = {};
			constantBufferResources.pSysMem = &consts;
			HRESULT result = GetDevice(graphics)->CreateBuffer(&constantBufferSettings, &constantBufferResources, &constantBuffer);
			LOG_DX_ERROR(result);
		}

		/* Default constructor to setup default buffer with no constants. */
		ConstantBuffer(Graphics& graphics)
		{
			// Create the buffer in the D3D device...
			D3D11_BUFFER_DESC constantBufferSettings;
			constantBufferSettings.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferSettings.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferSettings.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferSettings.MiscFlags = 0u;
			constantBufferSettings.ByteWidth = sizeof(C);
			constantBufferSettings.StructureByteStride = 0u;
			HRESULT result = GetDevice(graphics)->CreateBuffer(&constantBufferSettings, nullptr, &constantBuffer);
			LOG_DX_ERROR(result);
		}

	protected:

		// Created constant buffer pointer.
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	};

	/* Vertex constant buffer class derived from the base class. */
	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
	public:

		/* Overridden bind function for binding a vertex constant buffer to the render pipeline. */
		using ConstantBuffer<C>::ConstantBuffer;
		virtual void Add(Graphics& graphics) noexcept override
		{
			ContextData::GetContext(graphics)->VSSetConstantBuffers(0u, 1u, ConstantBuffer<C>::constantBuffer.GetAddressOf());
		}
	};

	/* Pixel constant buffer class derived from the base class. */
	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
	public:

		/* Overridden bind function for binding a constant constant buffer to the render pipeline. */
		using ConstantBuffer<C>::ConstantBuffer;
		virtual void Add(Graphics& graphics) noexcept override
		{
			ContextData::GetContext(graphics)->PSSetConstantBuffers(0u, 1u, ConstantBuffer<C>::constantBuffer.GetAddressOf());
		}
	};
}