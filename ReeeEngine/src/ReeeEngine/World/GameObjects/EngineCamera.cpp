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
		SetWorldLocation(Vector3D(0.0f, 0.0f, -10.0f));
	}
	
	EngineCamera::~EngineCamera()
	{
		GameObject::~GameObject();
	
	
	}
	
	void EngineCamera::LevelStart()
	{
		GameObject::LevelStart();

	}
	
	void EngineCamera::Tick(float DeltaTime)
	{
		GameObject::Tick(DeltaTime);

		// Handle input for the camera.
		Window& engineWindow = Application::GetEngine().GetWindow();
		WindowsInput& inputComponent = engineWindow.input;
		if (inputComponent.IsMouseDown(EMouseButton::Left))
		{
			// Handle camera mouse movement.
			const Vector2D currMousePos = inputComponent.GetMousePosition();
			if (inputComponent.IsMouseMoving() && !firstMovement)
			{
				Vector2D movementAmount = (currMousePos - lastMousePos).GetNormal() * mouseSpeed;
				SetWorldRotation(Rotator(movementAmount.Y, movementAmount.X, 0.0f), true);
			}

			// Increase movement speed while shift is held.
			float newSpeed = movementSpeed;
			if (inputComponent.IsKeyDown(KEY_SHIFT)) newSpeed *= 5.0f;

			// Handle positional movement of the camera.
			if (inputComponent.IsKeyDown(KEY_W))
			{
				SetWorldLocation(GetWorldTransform().GetForwardVector() * newSpeed, true);
			}
			else if (inputComponent.IsKeyDown(KEY_S))
			{
				SetWorldLocation(GetWorldTransform().GetForwardVector() * -newSpeed, true);
			}
			if (inputComponent.IsKeyDown(KEY_D))
			{
				SetWorldLocation(GetWorldTransform().GetRightVector() * newSpeed, true);
			}
			else if (inputComponent.IsKeyDown(KEY_A))
			{
				SetWorldLocation(GetWorldTransform().GetRightVector() * -newSpeed, true);
			}
			if (inputComponent.IsKeyDown(KEY_SPACE))
			{
				SetWorldLocation(Vector3D::GetWorldUpVector() * newSpeed, true);
			}
			else if (inputComponent.IsKeyDown(KEY_CONTROL))
			{
				SetWorldLocation(Vector3D::GetWorldUpVector() * -newSpeed, true);
			}

			// Update last mouse location.
			lastMousePos = currMousePos;
			firstMovement = false;
		}
		else
		{
			// Reset viewport.
			firstMovement = true;
		}
	}
}

