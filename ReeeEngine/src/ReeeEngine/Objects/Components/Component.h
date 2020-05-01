#pragma once
#include "../Core/Object.h"

namespace ReeeEngine
{
	/* Components can be attached to each other or a GameObject as its root component.
	 * NOTE: They cannot exist without an owning game object. */
	class REEE_API Component : Object
	{
	public:

		/* Setup default component. */
		Component(const std::string componentName);
		~Component() = default;

		/* Ticking function. */
		virtual void Tick(float DeltaTime) override;



	};
}

