#include "SampleState.h"

namespace ReeeEngine
{
	SampleState::SampleState(Graphics& graphics)
	{
		// Create sampler default options for UV read type.
		D3D11_SAMPLER_DESC samplerOptions = {};
		samplerOptions.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerOptions.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerOptions.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerOptions.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		
		// Create sampler state and check/log errors.
		HRESULT result = GetDevice(graphics)->CreateSamplerState(&samplerOptions, &sampler);
		LOG_DX_ERROR(result);
	}

	void SampleState::Add(Graphics& graphics) noexcept
	{
		// Add sampler to rendering pipeline.
		GetContext(graphics)->PSSetSamplers(0, 1, sampler.GetAddressOf());
	}
}