#include "VertexData.h"

namespace ReeeEngine
{
	void VertextData::Add(Graphics& graphics) noexcept
	{
		const UINT offset = 0u;
		GetContext(graphics)->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
}
