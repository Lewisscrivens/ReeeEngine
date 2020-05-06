#include "World.h"
#include "Core/GameObject.h"
#include "Components/CameraComponent.h"
#include "GameObjects/EngineCamera.h"
#include "../Application.h"
#include "../Rendering/Lights/PointLight.h"
#include "GameObjects/StaticMeshObject.h"
#include "Components/MeshComponent.h"

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
		// Temp only supports one point-light default created can be moved.
		pointLight = new PointLight(Application::GetEngine().GetWindow().GetGraphics());

		// For each loaded object run level start.
		for (auto& obj : objects)
		{
			obj->LevelStart();
		}
	}

	void World::Tick(float deltaTime)
	{
		// Bind point light information to pipeline for mesh components to later access through the constant buffer.
		pointLight->Add(Application::GetEngine().GetWindow().GetGraphics(), GetActiveCamera().GetViewMatrix());

		// For each loaded object tick/render.
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

	void World::SetLightWorldPosition(const Vector3D& newPosition)
	{
		pointLight->SetPosition(newPosition);
	}
}

