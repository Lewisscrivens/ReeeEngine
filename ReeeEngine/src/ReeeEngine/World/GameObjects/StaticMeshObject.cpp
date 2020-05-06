#include "StaticMeshObject.h"
#include "..\Components\MeshComponent.h"

namespace ReeeEngine
{
	StaticMeshObject::StaticMeshObject(const std::string& name) : GameObject(name)
	{
		// Create mesh and setup attachment.
		staticMeshComponent = CreateSubobject<MeshComponent>("StaticMesh");
		staticMeshComponent->AttachToComponent(rootComponent.get());

		// NOTE: Need to run set static mesh on staticMeshComponent...
	}

	StaticMeshObject::~StaticMeshObject()
	{
		GameObject::~GameObject();


	}

	void StaticMeshObject::LevelStart()
	{
		GameObject::LevelStart();


	}

	void StaticMeshObject::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);


	}
}