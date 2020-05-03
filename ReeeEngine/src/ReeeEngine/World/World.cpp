#include "World.h"
#include "Core/GameObject.h"
#include "Components/CameraComponent.h"
#include "GameObjects/EngineCamera.h"

namespace ReeeEngine
{
	World::World()
	{
		// Initalise the editor camera object.
		engineCamera = NewObject<EngineCamera>("EngineCameraObject");
		activeCamera = &engineCamera->GetCamera();
	}

	World::~World()
	{
		// Destroy all objects loaded into the world.
		objects.clear();
	}

	void World::LevelStart()
	{
		// For each loaded object run level start.
		for (auto& obj : objects)
		{
			obj->LevelStart();
		}
	}

	void World::Tick(float deltaTime)
	{
		// For each loaded object tick.
		for (auto& obj : objects)
		{
			obj->Tick(deltaTime);
		}
	}

	CameraComponent& World::GetActiveCamera()
	{
		return *activeCamera;
	}

	void World::SetActiveCamera(CameraComponent* camera)
	{
		activeCamera = camera;
	}
}

