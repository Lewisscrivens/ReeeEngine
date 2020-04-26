#include "ContextData.h"

namespace ReeeEngine
{
	ID3D11DeviceContext* ContextData::GetContext(Graphics& graphics) noexcept
	{
		return graphics.context.Get();
	}

	ID3D11Device* ContextData::GetDevice(Graphics& graphics) noexcept
	{
		return graphics.device.Get();
	}
}