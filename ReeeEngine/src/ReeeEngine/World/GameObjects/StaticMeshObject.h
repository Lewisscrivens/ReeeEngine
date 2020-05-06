#pragma once
#include "..\..\Globals.h"
#include "..\Core\GameObject.h"

namespace ReeeEngine
{
	/* Static mesh world object to hold a static mesh. */
	class REEE_API StaticMeshObject : public GameObject
	{
	public:

		/* Constructor and destructor. */
		StaticMeshObject(const std::string& name);
		~StaticMeshObject();

		/* Level start function. */
		virtual void LevelStart() override;

		/* Ticking function. */
		virtual void Tick(float DeltaTime) override;

		/* Mesh component getter. */
		class MeshComponent& GetStaticMesh() { return *staticMeshComponent; };

	private:

		/* Pointer to the static mesh component owned by this object. */
		Pointer<MeshComponent> staticMeshComponent;
	};
}
