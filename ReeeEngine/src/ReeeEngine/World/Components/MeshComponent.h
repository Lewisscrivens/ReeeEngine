#pragma once
#include "SceneComponent.h"
#include "../../Globals.h"
#include "../../Rendering/Renderables/Mesh.h"

namespace ReeeEngine
{
	/* Component that can pass rendering information to the Direct3D pipeline. */
	class REEE_API MeshComponent : public SceneComponent
	{
	public:

		/* Default constructor/destructor. */
		MeshComponent(const std::string name);
		~MeshComponent() = default;

		/* Static mesh setting/initialization function. */
		void SetStaticMesh(const std::string& filePath, float importScale = 1.0f, bool lit = false);

		/* Override transform change call to update mesh. */
		virtual void TransformChanged() override;

		/* Level start function. */
		virtual void LevelStart() override;

		/* Ticking function. */
		virtual void Tick(float deltaTime) override;

	private:

		// Pointer to the static mesh held under this component.
		Refference<Mesh> staticMesh;
	};
}
