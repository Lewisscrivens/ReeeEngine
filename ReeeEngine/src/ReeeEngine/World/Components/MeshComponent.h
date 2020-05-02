#pragma once
#include "SceneComponent.h"
#include "../../Globals.h"

namespace ReeeEngine
{
	/* Component that can pass rendering information to the Direct3D pipeline. */
	class REEE_API MeshComponent : public SceneComponent
	{
	public:

		/* Default constructor/destructor. */
		MeshComponent(const std::string name);
		~MeshComponent() = default;

	};
}
