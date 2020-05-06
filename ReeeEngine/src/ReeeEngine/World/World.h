#pragma once
#include "../Globals.h"
#include "../ReeeLog.h"
#include "../Math/Vector3D.h"

namespace ReeeEngine
{
	/* Helper window macros for throwing errors into the log and stopping code execution. */
	#define WORLD_THROW_EXCEPT(...) { REEE_LOG(Error, "World Error: ", __VA_ARGS__); __debugbreak(); }
	#define WORLD_EXCEPT(result, ...) if(!result) { WORLD_THROW_EXCEPT(__VA_ARGS__); }

	/* Define types used. */
	class GameObject;

	/* World object to handle loading/updating of objects as well as
	 * setting up the sky box etc.  
	 * NOTE: Plans to make one world instance with level instances loaded from .txt files. */
	class REEE_API World
	{
	public:

		/* Default world initialization/destruction. */
		World();
		~World();

		/* Level begin function. */
		void LevelStart();

		/* Ticking function ran from application for updating game objects. */
		void Tick(float deltaTime);

		/* Current active camera getter and setters. */
		class CameraComponent& GetActiveCamera();
		void SetActiveCamera(CameraComponent* camera);

		/* Object spawning function for adding new game objects to the world. */
		template<class T>
		Pointer<T> NewObject(const std::string& name)
		{
			// Ensure the type of sub object being created is a type of component.
			WORLD_EXCEPT((std::is_base_of<GameObject, T>::value), "Attempting to create an object that is not of sub-type gameobject.");
			Pointer<T> newGameObject = CreatePointer<T>(name);
			objects.push_back(newGameObject);
			return newGameObject;
		}

		// TEMP LIGHT POSITIONING FUNCTION FOR DEMO GAME.
		void SetLightWorldPosition(const Vector3D& newPosition);

	private:

		// TEMP LIGHT FOR DEMO GAME.
		class PointLight* pointLight;

		// Array of intialised game objects.
		std::vector<Pointer<GameObject>> objects;

		// Pointers to the editor camera.
		Pointer<class EngineCamera> engineCamera;
		CameraComponent* activeCamera;
	};
}