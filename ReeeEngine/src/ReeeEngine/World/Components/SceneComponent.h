#pragma once
#include "Component.h"
#include "../../Math/ReeeMath.h"
#include "../../Math/Transform.h"
#include "../../ReeeLog.h"

/* Helper window macros for throwing errors into the log and stopping code execution. */
#define COMPONENT_WARNING(...) REEE_LOG(Warning, "Component Warning: ", __VA_ARGS__)
#define COMPONENT_CHECK_RETURN(result, ...) if(!result) { COMPONENT_WARNING(__VA_ARGS__); return; }

namespace ReeeEngine
{
	enum class AttachmentRule
	{
		AttachWorld,	// Keep location/rotation/scale of component in the world relative to the new parent component.
		AttachRelative, // Setup new attachment to use current location/rotation/scale as relative offset to the new parent component.
		Snap			// Snap the component to the new parent components world (0,0,0)
	};

	/* Attachment properties for attaching a component to a new parent. 
	 * Note: by default it keeps all current world positions. */
	struct AttachProperties
	{
	public:
		// Setup default properties and constructors.
		AttachmentRule locationRule;
		AttachmentRule rotationRule;
		AttachmentRule scaleRule;

		AttachProperties(const AttachmentRule& LocRule = AttachmentRule::AttachWorld, const AttachmentRule& RotRule = AttachmentRule::AttachWorld, const AttachmentRule& ScaleRule = AttachmentRule::AttachWorld)
		{
			locationRule = LocRule;
			rotationRule = RotRule;
			scaleRule = ScaleRule;
		}
	};

	/* A Component that has a position relative to its owning component in the world. */
	class REEE_API SceneComponent : public Component
	{
	public:

		/* Setup default component. */
		SceneComponent(const std::string componentName);
		~SceneComponent() = default;

		/* Level start function. */
		virtual void LevelStart() override;

		/* Ticking function. */
		virtual void Tick(float DeltaTime) override;

		/* Add and remove function for children components attached to this component. */
		void AddChild(SceneComponent* child);
		void RemoveChild(SceneComponent* child);

		/* Owning component getter and setter. */
		class SceneComponent* GetAttachParent() const;
		void AttachToComponent(SceneComponent* newParent, const AttachProperties& props = AttachProperties());

		/* Recursive positioning functions for updating attachment position/rotation/scale hierarchy. */
		void InitNewLocation(const Vector3D& change);
		void InitNewRotation(const Rotator& change);
		void InitNewScale(const Vector3D& change);

		/* Location manipulation functions. */
		Vector3D GetWorldLocation() const;
		Vector3D GetRelativeLocation() const;
		void SetWorldLocation(const Vector3D& newLocation, bool addToCurrent = false);
		void SetRelativeLocation(const Vector3D& newRelativeLocation, bool addToCurrent = false);

		/* Rotation manipulation functions. */
		Rotator GetWorldRotation() const;
		Rotator GetRelativeRotation() const;
		void SetWorldRotation(const Rotator& newRotation, bool addToCurrent = false);
		void SetRelativeRotation(const Rotator& newRelativeRotation, bool addToCurrent = false);

		/* Scale manipulation functions. */
		Vector3D GetWorldScale() const;
		Vector3D GetRelativeScale() const;
		void SetWorldScale(const Vector3D& newScale, bool addToCurrent = false);
		void SetRelativeScale(const Vector3D& newRelativeScale, bool addToCurrent = false);

		/* Transform manipulation functions. */
		Transform GetWorldTransform() const;
		Transform GetRelativeTransform() const;
		void SetWorldTransform(const Transform& newTransform);
		void SetRelativeTransform(const Transform& newRelativeTransform);

		/* Get vector array of attached children components. */
		std::vector<SceneComponent*> GetChildren() const;

		/* Get number of attached children components. */
		int GetChildrenNum() const;

	protected:

		// The scene component this component is attached to.
		SceneComponent* attachParent;

		// Vector array of children components attached to this component.
		std::vector<SceneComponent*> childrenComponents;

		// The components world transform.
		Transform transform;
	};
}