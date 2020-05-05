#pragma once
#include "Object.h"
#include "../../ReeeLog.h"
#include "../../Math/Vector3D.h"
#include "../../Math/Rotator.h"
#include "../../Math/Transform.h"
#include "../Components/Component.h"

/* Define used classes. */
class SceneComponent;

/* Helper window macros for throwing errors into the log and stopping code execution. */
#define OBJECT_THROW_EXCEPT(...) { REEE_LOG(Error, "GameObject Error: ", __VA_ARGS__); __debugbreak(); }
#define OBJECT_EXCEPT(result, ...) if(!result) { OBJECT_THROW_EXCEPT(__VA_ARGS__); }

namespace ReeeEngine
{
	/* Game objects are world objects that own components. */
	class REEE_API GameObject : public Object
	{
	public:

		/* Setup default game object. */
		GameObject(const std::string objectName);
		~GameObject();

		/* Level start function. */
		virtual void LevelStart() override;

		/* Ticking function. */
		virtual void Tick(float DeltaTime) override;

		/* Getters for returning world rotation, scale and rotation from the set root component. */
		Vector3D GetWorldLocation() const;
		Rotator GetWorldRotation() const;
		Vector3D GetWorldScale() const;
		Transform GetWorldTransform() const;

		/* Setters for world rotation, scale and rotation from the set root component. */
		void SetWorldLocation(const Vector3D& newLocation, bool addToCurrent = false);
		void SetWorldRotation(const Rotator& newRotation, bool addToCurrent = false);
		void SetWorldScale(const Vector3D& newScale, bool addToCurrent = false);
		void SetWorldTransform(const Transform& newTransform);

		/* Get the root component. */
		Pointer<SceneComponent> GetRootComponent() const { return rootComponent; }

		/* Get number of children owned by this object. */
		int GetNumChildren() const;

	protected:

		/* Declare and define create sub-object function for adding new components to a given game object. */
		template<class T>
		Pointer<T> CreateSubobject(const std::string& compName)
		{
			// Ensure the type of sub object being created is a type of component.
			OBJECT_EXCEPT((std::is_base_of<Component, T>::value), "Attempting to create a Subobject of a non component type.");

			// Setup and attach new component.
			Pointer<T> newComp = CreatePointer<T>(compName);
			Component* comp = static_cast<Component*>(newComp.get());
			comp->SetOwner(this);
			components.push_back(comp);
			return newComp;
		}

		// Root comp pointer. NOTE: Auto intialised for each game object as a scene component.
		Pointer<SceneComponent> rootComponent;

		// Array of all of subobjects owned by this game object.
		std::vector<Component*> components;
	};
}
