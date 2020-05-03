#include "CameraComponent.h"
#include "SceneComponent.h"

namespace ReeeEngine
{
	CameraComponent::CameraComponent(const std::string& name) : SceneComponent(name)
	{

	}

	CameraComponent::~CameraComponent()
	{

	}

	void CameraComponent::LevelStart()
	{
		SceneComponent::LevelStart();

	}

	void CameraComponent::Tick(float deltaTime)
	{
		SceneComponent::Tick(deltaTime);

	}

	void CameraComponent::SetWindowSize(float width, float height)
	{
		projectionSettings.width = width;
		projectionSettings.height = height;
		SetProjectionSettings();
	}

	DirectX::XMMATRIX CameraComponent::GetViewMatrix() const
	{
		// Get the base forward vector.
		const DirectX::XMVECTOR Forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		// Apply camera rotations to the base forward vector.
		Rotator worldRotation = GetWorldRotation().ToRadians();
		Vector3D worldPosition = GetWorldLocation();
		DirectX::XMVECTOR lookVector = DirectX::XMVector3Transform(Forward, DirectX::XMMatrixRotationRollPitchYaw(worldRotation.Pitch, worldRotation.Yaw, worldRotation.Roll));

		// Get the cameras position from the world location vector 3d.
		DirectX::XMVECTOR cameraPosition = DirectX::XMLoadFloat3(&worldPosition.ToFloat3());

		// Get the direction the camera is looking in.
		DirectX::XMFLOAT3 float3LookVector;
		DirectX::XMFLOAT3 camTarget;
		DirectX::XMStoreFloat3(&float3LookVector, lookVector);
		camTarget.x = worldPosition.X + float3LookVector.x;
		camTarget.y = worldPosition.Y + float3LookVector.y;
		camTarget.z = worldPosition.Z + float3LookVector.z;

		// Get the up vector.
		DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR worldRotationVector = DirectX::XMQuaternionRotationRollPitchYaw(worldRotation.Pitch, worldRotation.Yaw, worldRotation.Roll);

		// Return the view matrix.
		return DirectX::XMMatrixLookAtLH(cameraPosition, DirectX::XMLoadFloat3(&camTarget), DirectX::XMVector3Rotate(upDirection, worldRotationVector));
	}

	void CameraComponent::SetProjectionSettings(float fov, float newWidth, float newHeight, float nearClip, float farClip) noexcept
	{
		// Update current projection settings if changed.
		if (fov != 0.0f) projectionSettings.fov = fov;
		if (newWidth != 0.0f) projectionSettings.width = newWidth;
		if (newHeight != 0.0f) projectionSettings.height = newHeight;
		if (nearClip != 0.0f) projectionSettings.nearClip = nearClip;
		if (farClip != 0.0f) projectionSettings.farClip = farClip;

		// Set new matrix.
		auto newMatrix = DirectX::XMMatrixPerspectiveFovLH(ReeeMath::Radians(projectionSettings.fov), ReeeMath::GetAspectRatio(projectionSettings.width, projectionSettings.height), projectionSettings.nearClip, projectionSettings.farClip);
		SetProjectionMatrix(newMatrix);
	}

	void CameraComponent::SetProjectionMatrix(DirectX::FXMMATRIX projectionMat) noexcept
	{
		projectionMatrix = projectionMat;
	}

	DirectX::XMMATRIX CameraComponent::GetProjectionMatrix() const noexcept
	{
		return projectionMatrix;
	}
}
