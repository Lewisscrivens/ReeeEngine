#pragma once
#include "Vector3D.h"
#include "Rotator.h"
#include "ReeeMath.h"

namespace ReeeEngine
{
	/* Transform struct declaration for holding the location, rotation and scale of something. */
	struct Transform
	{
	private:

		/* Structure variables. */
		Vector3D currLocation;
		Vector3D currScale;
		Rotator currRotation;

	public:

		/* Default constructor's. */
		Transform() : currLocation(Vector3D(0.0f)), currRotation(Rotator(0.0f)), currScale(Vector3D(1.0f)) {}
		Transform(Vector3D location, Rotator rotation, Vector3D scale) : currLocation(location), currRotation(rotation), currScale(scale) {}

		/* Get location of transform. */
		Vector3D GetLocation() const
		{
			return currLocation;
		}

		/* Get rotation of transform. */
		Rotator GetRotation() const
		{
			return currRotation;
		}

		/* Get scale of transform. */
		Vector3D GetScale() const
		{
			return currScale;
		}

		/* Set location of transform. */
		void SetLocation(const Vector3D& newLocation)
		{
			currLocation = newLocation;
		}

		/* Set rotation of transform. */
		void SetRotation(const Rotator& newRotation)
		{
			currRotation = newRotation;
		}

		/* Set scale of transform. */
		void SetScale(const Vector3D& newScale)
		{
			currScale = newScale;
		}

		/* Get the DirectX transform from this vector for performing matrix calculations. */
		DirectX::XMMATRIX GetTransformAsMatrix()
		{
			Rotator rotRad = currRotation.ToRadians();
			return DirectX::XMMatrixScaling(currScale.X, currScale.Y, currScale.Z) * DirectX::XMMatrixRotationRollPitchYaw(rotRad.Pitch, rotRad.Yaw, rotRad.Roll) *
				DirectX::XMMatrixTranslation(currLocation.X, currLocation.Y, currLocation.X);
		}

		/* Setup this transform from a given matrix transform. */
		void SetTransformFromMatrix(const DirectX::XMMATRIX& newMatrix)
		{
			// Get vector values for location rotation and scale from the given matrix.
			DirectX::XMVECTOR loc, rot, scal;
			DirectX::XMMatrixDecompose(&loc, &rot, &scal, newMatrix);
			
			// Update location values.
			DirectX::XMFLOAT4 location;
			DirectX::XMStoreFloat4(&location, loc);
			currLocation = Vector3D(location.x, location.y, location.z);

			// Update rotation values convert back to degrees before updating values.
			DirectX::XMFLOAT4 rotation;
			DirectX::XMStoreFloat4(&rotation, rot);
			Rotator currRotationRad = Rotator(rotation.x, rotation.y, rotation.z);
			currRotation = currRotationRad.ToDegrees();

			// Update scale values.
			DirectX::XMFLOAT4 scale;
			DirectX::XMStoreFloat4(&scale, scal);
			currScale = Vector3D(scale.x, scale.y, scale.z);
		}
	};
}