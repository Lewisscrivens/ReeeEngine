#include "Topology.h"

namespace ReeeEngine
{
	Topology::Topology(Graphics& graphics, D3D11_PRIMITIVE_TOPOLOGY topType)
	{
		type = topType;
	}

	void Topology::Add(Graphics& graphics) noexcept
	{
		GetContext(graphics)->IASetPrimitiveTopology(type);
	}
}