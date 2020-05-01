#pragma once
#include "Component.h"

namespace ReeeEngine
{
	/* A Component that has a position relative to its owning component in the world. */
	class REEE_API SceneComponent : Component
	{
	public:

		/* Setup default component. */
		SceneComponent(const std::string componentName);
		~SceneComponent() = default;

		/* Ticking function. */
		virtual void Tick(float DeltaTime) override;




	};
}