#include "SceneComponent.h"

namespace ReeeEngine
{
	SceneComponent::SceneComponent(const std::string componentName) : Component(componentName)
	{

	}

	void SceneComponent::Tick(float DeltaTime) : Component::Tick(DeltaTime)
	{
		//...
	}


}

