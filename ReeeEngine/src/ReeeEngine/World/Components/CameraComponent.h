#pragma once
#include "../../Globals.h"
#include "../../Math/ReeeMath.h"
#include "SceneComponent.h"

namespace ReeeEngine
{
	/* Define the default projection settings. */
	struct ProjectionSettings
	{
		float fov = 60.0f;
		float width = 1280.0f;
		float height = 720.0f;
		float nearClip = 0.5f;
		float farClip = 2000.0f;
	};

	/* A camera component to view the game world. */
	class REEE_API CameraComponent : public SceneComponent
	{
	public:

		/* Default constructor. */
		CameraComponent(const std::string& name = "Camera");
		~CameraComponent();

		/* Component function overrides. */
		virtual void LevelStart() override;
		virtual void Tick(float deltaTime) override;

		/* Returns view matrix. */
		DirectX::XMMATRIX GetViewMatrix() const;

		/* Projection matrix functions for changing camera's FOV, far and near clip planes and aspect ratio from height and width of monitor. */
		void SetWindowSize(float width, float height);
		void SetProjectionSettings(float fov = 0.0f, float newWidth = 0.0f, float newHeight = 0.0f, float nearClip = 0.0f, float farClip = 0.0f) noexcept;
		void SetProjectionMatrix(DirectX::FXMMATRIX projectionMat) noexcept;
		DirectX::XMMATRIX GetProjectionMatrix() const noexcept;

	private:

		/* Saved projection matrix settings. */
		ProjectionSettings projectionSettings;

		/* Current projection matrix. */
		DirectX::XMMATRIX projectionMatrix;
	};
}