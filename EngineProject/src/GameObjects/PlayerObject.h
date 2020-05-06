#pragma once
#include "..\..\World\GameObjects\StaticMeshObject.h"
#include "..\..\World\Components\CameraComponent.h"

/* The player object for the driving game demo. */
class PlayerObject : public ReeeEngine::StaticMeshObject
{
public:

	/* Constructor and destructor. */
	PlayerObject(const std::string& name);
	~PlayerObject();

	/* Level start function. */
	virtual void LevelStart() override;

	/* Ticking function. */
	virtual void Tick(float deltaTime) override;

private:

	// Movement variables.
	float driveSpeed = 15.0f;
};

