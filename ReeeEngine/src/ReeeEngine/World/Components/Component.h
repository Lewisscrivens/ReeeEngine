#pragma once
#include "../Core/Object.h"

namespace ReeeEngine
{
	/* Components can be attached to each other or a GameObject as its root component.
	 * NOTE: They cannot exist without an owning game object. */
	class REEE_API Component : public Object
	{
	public:

		/* Setup default component. */
		Component(const std::string componentName);
		~Component() = default;

		/* Level start function. */
		virtual void LevelStart() override;

		/* Ticking function. */
		virtual void Tick(float deltaTime) override;

		/* Owning game object getter and setter. */
		class GameObject* GetOwner() const;
		void SetOwner(GameObject* newOwner);

	protected:

		/* Pointers to parent object. */
		GameObject* owner;
	};
}

