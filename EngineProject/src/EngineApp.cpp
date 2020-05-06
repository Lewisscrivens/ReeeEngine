#include "EngineApp.h"
#include "ReeeEngine/ReeeLog.h"
#include "../../World/World.h"
#include "../../World/Components/MeshComponent.h"
#include "../../World/Components/CameraComponent.h"
#include "../../World/Core/GameObject.h"

using namespace ReeeEngine;
EngineApp::EngineApp()
{
	//...
}

EngineApp::~EngineApp()
{
	//...
}

void EngineApp::Init()
{
	Application::Init();

	// Load skybox.
	skybox = GetWorld()->NewObject<ReeeEngine::StaticMeshObject>("SkyboxObject");
	skybox->GetStaticMesh().SetStaticMesh("../Assets/skybox", -500.0f, true);

	// Load in the road.
	road = GetWorld()->NewObject<ReeeEngine::StaticMeshObject>("RoadObject");
	road->GetStaticMesh().SetStaticMesh("../Assets/RoadMesh", 0.7f, false);
	road->SetWorldRotation(Rotator(0.0f, 90.0f, 0.0f));

	// Loading the player object into the scene.
	// Attach the engine camera as I do not have a multi camera system implemented...
	player = GetWorld()->NewObject<PlayerObject>("PlayerObject");
	GetWorld()->GetActiveCamera().AttachToComponent(player->GetRootComponent().get());
	GetWorld()->GetActiveCamera().SetWorldLocation(Vector3D(0.0f, 2.0f, -5.0f));
	GetWorld()->GetActiveCamera().SetWorldRotation(Rotator(8.0f, 0.0f, 0.0f));

	// Log initialisation.
	REEE_LOG(Log, "Assets for game demo loaded succesfully.");
}

void EngineApp::Tick(float deltaTime)
{
	Application::Tick(deltaTime);

	// If player has been intialised update the engine light to the car.
	if (player)
	{
		Transform playerTransform = player->GetWorldTransform();
		GetWorld()->SetLightWorldPosition((playerTransform.GetLocation() + Vector3D(0.0f, 2.0f, 0.0f)) + (playerTransform.GetForwardVector() * 15.0f));
		skybox->SetWorldLocation(playerTransform.GetLocation());

		// Check the OpenCV input and add rotation to the car.
		if (GetOpenCVInput() && GetOpenCVInput()->IsInitialised())
		{
			float turnSpeed = 1.0f;
			player->SetWorldRotation(Rotator(0.0f, GetOpenCVInput()->GetHeightDiffernce() * turnSpeed, 0.0f), true);
			
			// Clamp rotation of car.
			Rotator currPlayerRot = player->GetWorldRotation();
			float clampedRot = ReeeEngine::ReeeMath::Clamp(currPlayerRot.Yaw, -20.0f, 20.0f);
			player->SetWorldRotation(Rotator(currPlayerRot.Pitch, clampedRot, currPlayerRot.Roll));

			// Update camera gimbal.
			GetWorld()->GetActiveCamera().SetWorldLocation(player->GetWorldLocation() + Vector3D(0.0f, 2.0f, -5.0f));
			GetWorld()->GetActiveCamera().SetWorldRotation(Rotator(8.0f, 0.0f, 0.0f));
		}

		// Clamp the player within the barriers.
		Vector3D currPlayerLoc = player->GetWorldLocation();
		float clampedPosition = ReeeEngine::ReeeMath::Clamp(currPlayerLoc.X, -2.0f, 2.0f);
		player->SetWorldLocation(Vector3D(clampedPosition, currPlayerLoc.Y, currPlayerLoc.Z));
	}
}
