#include "Component.h"
#include "SceneComponent.h"
#include "../Core/GameObject.h"
#include "../../ReeeLog.h"

namespace ReeeEngine
{
	Component::Component(const std::string componentName) : Object(componentName)
	{
		// Initalise to null by default.
		owner = nullptr;
	}

	void Component::LevelStart()
	{
		// Ensure the object has an owner before it is ticked.
		if (!owner) REEE_LOG(Warning, "Attempting to start game with the component {0} created with no owner.", name);



	}

	void Component::Tick(float DeltaTime)
	{
		//...
	}

	GameObject* Component::GetOwner() const
	{
		return owner;
	}

	void Component::SetOwner(GameObject* newOwner)
	{
		owner = owner;
	}
}