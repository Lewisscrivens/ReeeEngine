#pragma once
#include "../../Globals.h"
#include "Component.h"

namespace ReeeEngine
{
	class REEE_API TextComponent : public Component
	{
	public:

		/* Setup default component. */
		TextComponent(const std::string componentName);
		~TextComponent() = default;

		/* Level start function. */
		virtual void LevelStart() override;

		/* Ticking function. */
		virtual void Tick(float deltaTime) override;

	private:


	};
}



