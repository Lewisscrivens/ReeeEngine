#include "MeshComponent.h"
#include "..\..\Application.h"
#include "..\..\Rendering\Renderables\Mesh.h"

namespace ReeeEngine
{
	MeshComponent::MeshComponent(const std::string name) : SceneComponent(name) {}

	void MeshComponent::SetStaticMesh(const std::string& filePath, float importScale, bool lit)
	{
		// Create mesh on rendering pipeline.
		staticMesh = CreateReff<Mesh>(Application::GetEngine().GetWindow().GetGraphics(), filePath, importScale, lit);
	}

	void MeshComponent::TransformChanged()
	{
		// Update the meshes transform matrix from this component.
		if (staticMesh) staticMesh->SetTransform(GetWorldTransform().GetTransformAsMatrix());
	}

	void MeshComponent::LevelStart()
	{
		SceneComponent::LevelStart();

	}

	void MeshComponent::Tick(float deltaTime)
	{
		SceneComponent::Tick(deltaTime);

		// Render static mesh...
		if (staticMesh) staticMesh->Render(Application::GetEngine().GetWindow().GetGraphics());
	}
}