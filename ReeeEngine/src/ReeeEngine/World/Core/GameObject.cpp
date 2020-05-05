#include "GameObject.h"
#include "../Components/SceneComponent.h"

namespace ReeeEngine
{
	GameObject::GameObject(const std::string objectName) : Object(objectName)
	{
		// Create root component by default.
		rootComponent = CreateSubobject<SceneComponent>("RootComponent");
	}

	GameObject::~GameObject()
	{
		// Clear components.
		components.clear();
	}

	void GameObject::LevelStart()
	{
		// Call level start on each component.
		for (auto& comp : components)
		{
			comp->LevelStart();
		}
	}

	void GameObject::Tick(float DeltaTime)
	{
		// Tick each component.
		for (auto& comp : components)
		{
			comp->Tick(DeltaTime);
		}
	}

	Vector3D GameObject::GetWorldLocation() const
	{
		return rootComponent->GetWorldLocation();
	}

	Rotator GameObject::GetWorldRotation() const
	{
		return rootComponent->GetWorldRotation();
	}

	Vector3D GameObject::GetWorldScale() const
	{
		return rootComponent->GetWorldScale();
	}

	Transform GameObject::GetWorldTransform() const
	{
		return rootComponent->GetWorldTransform();
	}

	void GameObject::SetWorldLocation(const Vector3D& newLocation, bool addToCurrent)
	{
		rootComponent->SetWorldLocation(newLocation, addToCurrent);
	}

	void GameObject::SetWorldRotation(const Rotator& newRotation, bool addToCurrent)
	{
		rootComponent->SetWorldRotation(newRotation, addToCurrent);
	}

	void GameObject::SetWorldScale(const Vector3D& newScale, bool addToCurrent)
	{
		rootComponent->SetWorldScale(newScale, addToCurrent);
	}

	void GameObject::SetWorldTransform(const Transform& newTransform)
	{
		return rootComponent->SetWorldTransform(newTransform);
	}

	int GameObject::GetNumChildren() const
	{
		return components.size();
	}
}