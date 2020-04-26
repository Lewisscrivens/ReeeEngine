#pragma once
#include "ContextData.h"

namespace ReeeEngine
{
	/* Class for creating new topology and adding it to the rendering pipeline. */
	class Topology : public ContextData
	{
	public:

		/* Constructor for new topology being added to the graphics device. */
		Topology(Graphics& graphics, D3D11_PRIMITIVE_TOPOLOGY topType);

		/* Function to bind the new topology  */
		virtual void Add(Graphics& graphics) noexcept override;

	protected:

		D3D11_PRIMITIVE_TOPOLOGY type;// Type of primitive topology created.
	};
}

