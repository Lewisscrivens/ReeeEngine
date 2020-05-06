#include "PlayerObject.h"
#include "..\..\World\Components\MeshComponent.h"

PlayerObject::PlayerObject(const std::string& name) : ReeeEngine::StaticMeshObject(name)
{
	// Setup player mesh.
	GetStaticMesh().SetStaticMesh("../Assets/PlayerCar", 1.0f, false);
	GetStaticMesh().SetRelativeRotation(ReeeEngine::Rotator(0.0f, 90.0f, 0.0f));
}

PlayerObject::~PlayerObject()
{

}

void PlayerObject::LevelStart()
{
	ReeeEngine::StaticMeshObject::LevelStart();

}

void PlayerObject::Tick(float deltaTime)
{
	ReeeEngine::StaticMeshObject::Tick(deltaTime);

	// While the game loop is player move the car forward.
	SetWorldLocation(GetWorldTransform().GetForwardVector() * (driveSpeed * deltaTime), true);
}
