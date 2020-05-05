#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* The sample state of given topology to a texture. */ 
	class SampleState : public ContextData
	{
	public:
		/* Construct default sampler. */
		SampleState(Graphics& graphics);

		/* Add default sampler state to the rendering pipeline. */
		virtual void Add(Graphics& graphics) noexcept override;

	protected:

		// The current sampler state pointer.
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	};
}