#pragma once
#include "../../Globals.h"
#include "../Core/GameObject.h"
#include "../../Math/Vector2D.h"

namespace ReeeEngine
{
	/* Controlled camera object for moving around the editor. TEMP FOR TESTING NEW FEATURES. */
	class REEE_API EngineCamera : public GameObject 
	{
	public:
		
		/* Constructor and destructor. */
		EngineCamera(const std::string& name);
		~EngineCamera();
	
		/* Level start function. */
		virtual void LevelStart() override;
	
		/* Ticking function. */
		virtual void Tick(float DeltaTime) override;
	
		/* Camera component getter. */
		class CameraComponent& GetCamera() { return *camera; };
	
	private:
	
		/* Pointer to camera component. */
		Pointer<CameraComponent> camera;
	
		/* Default movement options. */
		bool firstMovement = true;
		Vector2D lastMousePos;
		float mouseSpeed = 1.0f;
		float movementSpeed = 0.2f;
	};
}
