#include "EngineCamera.h"
#include "../Components/CameraComponent.h"
#include "../../Application.h"
#include "../../Windows/WindowsKeyCodes.h"

namespace ReeeEngine
{
	EngineCamera::EngineCamera(const std::string& name) : GameObject(name)
	{
		camera = CreateSubobject<CameraComponent>("EditorCamera");
		camera->AttachToComponent(rootComponent.get());
		SetWorldLocation(Vector3D(0.0f, 0.0f, -50.0f));
	}
	
	EngineCamera::~EngineCamera()
	{
		GameObject::~GameObject();
	
	
	}
	
	void EngineCamera::LevelStart()
	{
	
	}
	
	void EngineCamera::Tick(float DeltaTime)
	{
		WindowsInput& inputComponent = Application::GetApp().GetWindow().input;
		if (inputComponent.IsMouseDown(EMouseButton::Left))
		{
			// Handle camera mouse movement.
			const Vector2D currMousePos = inputComponent.GetMousePosition();
			if (inputComponent.IsMouseMoving() && !firstMovement)
			{
				Vector2D movementAmount = (currMousePos - lastMousePos).GetNormal() * mouseSpeed;
				SetWorldRotation(Rotator(movementAmount.Y, movementAmount.X, 0.0f), true);
			}

			// Handle positional movement of the camera.
			if (inputComponent.IsKeyDown(KEY_W))
			{
				camera->SetRelativeLocation(Vector3D(0.0f, 0.0f, -10.0f));
			}

			// Update last mouse location.
			lastMousePos = currMousePos;
			firstMovement = false;
		}
		else firstMovement = true;
	}
}

