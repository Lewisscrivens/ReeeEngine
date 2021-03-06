#pragma once
#include "../../Globals.h"

namespace ReeeEngine
{
	/* Base object for anything that has to be ticked within the engine.
	 * NOTE: For example GameObjects and components will be the main subclasses. */
	class REEE_API Object
	{
		friend class SceneComponent;
		friend class GameObject;

	public:

		/* Default constructor and destructor. */
		Object(const std::string objectName) : name(objectName) {}
		~Object() = default;

		/* Level start function. */
		virtual void LevelStart() = 0;

		/* Ticking function for updating the object per frame. */
		virtual void Tick(float DeltaTime) = 0;

		/* Return the objects name. */
		const std::string GetName() { return name; }

	protected:

		// The name of this object.
		std::string name;
	};
}
