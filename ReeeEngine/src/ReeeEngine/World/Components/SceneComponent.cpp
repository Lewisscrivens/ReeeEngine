#include "SceneComponent.h"

namespace ReeeEngine
{
	SceneComponent::SceneComponent(const std::string componentName) : Component(componentName)
	{
		attachParent = nullptr;
	}

	void SceneComponent::LevelStart()
	{
		Component::LevelStart();

		//...
	}

	void SceneComponent::Tick(float DeltaTime)
	{
		Component::Tick(DeltaTime);

		//...
	}

	SceneComponent* SceneComponent::GetAttachParent() const
	{
		return attachParent;
	}

	void SceneComponent::AddChild(SceneComponent* child)
	{
		COMPONENT_CHECK_RETURN(child, "AddChild: child is null cannot add returning...");
		childrenComponents.push_back(child);
	}

	void SceneComponent::RemoveChild(SceneComponent* child)
	{
		COMPONENT_CHECK_RETURN(child, "RemoveChild: child is null cannot remove returning...");
		
		// Find child and remove from array if it is there.
		auto it = std::find(childrenComponents.begin(), childrenComponents.end(), child);
		if (it != childrenComponents.end()) 
		{ 
			childrenComponents.erase(it); 
		}
	}

	void SceneComponent::AttachToComponent(SceneComponent* newParent, const AttachProperties& props)
	{
		COMPONENT_CHECK_RETURN(newParent, "AttachToComponent: newParent is null cannot attach.");

		// If already attached clear old parent and remove child from children list.
		if (attachParent)
		{
			attachParent->RemoveChild(this);
			attachParent = nullptr;
		}

		// Set new attach parent.
		attachParent = newParent;

		// Add to children list of parent.
		newParent->AddChild(this);

		// Setup location rules.
		switch (props.locationRule)
		{
			case AttachmentRule::AttachRelative:
			{
				SetRelativeLocation(GetWorldLocation());
				break;
			}
			case AttachmentRule::Snap:
			{
				SetWorldLocation(newParent->GetWorldLocation());
				break;
			}
		}

		// Setup rotation rules.
		switch (props.rotationRule)
		{
			case AttachmentRule::AttachRelative:
			{
				SetRelativeRotation(GetWorldRotation());
				break;
			}
			case AttachmentRule::Snap:
			{
				SetWorldRotation(newParent->GetWorldRotation());
				break;
			}
		}

		// Setup scale rules.
		switch (props.scaleRule)
		{
			case AttachmentRule::AttachRelative:
			{
				SetRelativeScale(GetWorldScale());
				break;
			}
			case AttachmentRule::Snap:
			{
				SetWorldScale(newParent->GetWorldScale());
				break;
			}
		}
	}

	void SceneComponent::InitNewLocation(const Vector3D& change)
	{
		for (auto it = childrenComponents.begin(); it != childrenComponents.end(); ++it)
		{
			// Update childrens world location from change.
			SceneComponent* currComp = *it;
			currComp->SetWorldLocation(change, true);
		}
	}

	void SceneComponent::InitNewRotation(const Rotator& change)
	{
		for (auto it = childrenComponents.begin(); it != childrenComponents.end(); ++it)
		{
			// Update childrens components rotations from rotation change in this parent.
			SceneComponent* currComp = *it;
			currComp->SetWorldRotation(change, true);

			// Update new world positions after a rotation is applied to the children.
			currComp->SetRelativeLocation(currComp->GetRelativeLocation());
		}
	}

	void SceneComponent::InitNewScale(const Vector3D& change)
	{
		for (auto it = childrenComponents.begin(); it != childrenComponents.end(); ++it)
		{
			// Update childrens world scale from change.
			SceneComponent* currComp = *it;
			currComp->SetWorldScale(change, true);
		}
	}

	Vector3D SceneComponent::GetWorldLocation() const
	{
		return transform.GetLocation();
	}

	Vector3D SceneComponent::GetRelativeLocation() const
	{
		// Return the relative transform to the attach parent if there is one.
		if (attachParent)
		{
			return relativeTransform.GetLocation();
		}
		// If there is no parent just return the world location...
		else return transform.GetLocation();
	}

	void SceneComponent::SetWorldLocation(const Vector3D& newLocation, bool addToCurrent)
	{
		if (addToCurrent && newLocation.IsZero()) return;
		const Vector3D newLoc = addToCurrent ? transform.GetLocation() + newLocation : newLocation;
		const Vector3D diff = newLoc - GetWorldLocation();
		transform.SetLocation(newLoc);
		InitNewLocation(diff);
	}

	void SceneComponent::SetRelativeLocation(const Vector3D& newRelativeLocation, bool addToCurrent)
	{
		if (addToCurrent && newRelativeLocation.IsZero()) return;
		if (attachParent)
		{
			// Get new relative location in terms of the parents rotation.
			const Vector3D relativeOffset = addToCurrent ? relativeTransform.GetLocation() + newRelativeLocation : newRelativeLocation;
			DirectX::XMVECTOR newRelVector = DirectX::XMLoadFloat3(&relativeOffset.ToFloat3());
			Rotator parentRotation = attachParent->GetWorldRotation().ToRadians();
			DirectX::XMVECTOR parentRotationVec = DirectX::XMQuaternionRotationRollPitchYaw(parentRotation.Pitch, parentRotation.Yaw, parentRotation.Roll);
			DirectX::XMVECTOR relativeWithRot = DirectX::XMVector3Rotate(newRelVector, parentRotationVec);

			// Get readable relative location with rotation.
			DirectX::XMFLOAT3 rotatedRelativeLocation;
			DirectX::XMStoreFloat3(&rotatedRelativeLocation, relativeWithRot);

			// Get new relative location and set the relative transform...
			Vector3D relativeWorldLocation = Vector3D(rotatedRelativeLocation.x, rotatedRelativeLocation.y, rotatedRelativeLocation.z);
			relativeTransform.SetLocation(relativeOffset);
			SetWorldLocation(attachParent->GetWorldLocation() + relativeWorldLocation);
		}
		else
		{
			// If theres no parent just update world location.
			SetWorldLocation(newRelativeLocation, addToCurrent);
		}
	}

	Rotator SceneComponent::GetWorldRotation() const
	{
		return transform.GetRotation();
	}

	Rotator SceneComponent::GetRelativeRotation() const
	{
		// Return world rotation if relative doesn't exist.
		return attachParent ? relativeTransform.GetRotation() : transform.GetRotation();
	}

	void SceneComponent::SetWorldRotation(const Rotator& newRotation, bool addToCurrent)
	{
		if (addToCurrent && newRotation.IsZero()) return;
		const Rotator newRot = addToCurrent ? transform.GetRotation() + newRotation : newRotation;
		const Rotator diff = newRot - GetWorldRotation();
		transform.SetRotation(newRot);
		InitNewRotation(diff);
	}

	void SceneComponent::SetRelativeRotation(const Rotator& newRelativeRotation, bool addToCurrent)
	{
		if (addToCurrent && newRelativeRotation.IsZero()) return;
		if (attachParent)
		{
			// Set world rotation from relative.
			Rotator relativeOffset = addToCurrent ? relativeTransform.GetRotation() + newRelativeRotation : newRelativeRotation;
			relativeTransform.SetRotation(relativeOffset);
			SetWorldRotation(attachParent->GetWorldRotation() + relativeOffset);
		}
		else
		{
			// If there is no parent just set world rotation instead.
			SetWorldRotation(newRelativeRotation, addToCurrent);
		}
	}

	Vector3D SceneComponent::GetWorldScale() const
	{
		return transform.GetScale();
	}

	Vector3D SceneComponent::GetRelativeScale() const
	{
		// Return world scale if relative doesn't exist.
		return attachParent ? (transform.GetScale() - attachParent->GetWorldTransform().GetScale()) : transform.GetScale();
	}

	void SceneComponent::SetWorldScale(const Vector3D& newScale, bool addToCurrent)
	{
		if (addToCurrent && newScale.IsZero()) return;
		const Vector3D newS = addToCurrent ? transform.GetScale() + newScale : newScale;
		const Vector3D diff = newS - GetWorldScale();
		transform.SetScale(newS);
		InitNewScale(diff);
	}

	void SceneComponent::SetRelativeScale(const Vector3D& newRelativeScale, bool addToCurrent)
	{
		if (addToCurrent && newRelativeScale.IsZero()) return;
		if (attachParent)
		{
			// Set world scale from relative.
			SetWorldScale(GetRelativeScale() + newRelativeScale, addToCurrent);
		}
		else
		{
			// Set the world scale if there is no parent comp.
			SetWorldScale(newRelativeScale, addToCurrent);
		}
	}

	Transform SceneComponent::GetWorldTransform() const
	{
		return transform;
	}

	Transform SceneComponent::GetRelativeTransform() const
	{
		return Transform(GetRelativeLocation(), GetRelativeRotation(), GetRelativeScale());
	}

	void SceneComponent::SetWorldTransform(const Transform& newTransform)
	{
		SetWorldLocation(newTransform.GetLocation());
		SetWorldRotation(newTransform.GetRotation());
		SetWorldScale(newTransform.GetScale());
	}

	void SceneComponent::SetRelativeTransform(const Transform& newRelativeTransform)
	{
		SetRelativeLocation(newRelativeTransform.GetLocation());
		SetRelativeRotation(newRelativeTransform.GetRotation());
		SetRelativeScale(newRelativeTransform.GetScale());
	}

	std::vector<SceneComponent*> SceneComponent::GetChildren() const
	{
		return childrenComponents;
	}

	int SceneComponent::GetChildrenNum() const
	{
		return childrenComponents.size();
	}
}