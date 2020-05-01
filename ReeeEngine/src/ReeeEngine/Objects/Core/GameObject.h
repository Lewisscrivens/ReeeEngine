#pragma once
#include "Object.h"

namespace ReeeEngine
{
	/* Game objects are world objects that own components. */
	class REEE_API GameObject : Object
	{
	public:

		/* Setup default game object. */
		GameObject(const std::string objectName);
		~GameObject() = default;

		/* Ticking function. */
		virtual void Tick(float DeltaTime) override;



	};
}
