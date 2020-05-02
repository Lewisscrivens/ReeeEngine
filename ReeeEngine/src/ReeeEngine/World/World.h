#pragma once
#include "../Globals.h"

namespace ReeeEngine
{
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

		/* Object spawning function for adding new game objects to the world. */
		template<class T>
		Pointer<T> SpawnWorldObject(const std::string& name)
		{
			Pointer<T> newGameObject = CreatePointer<T>(name);
			objects.push_back(newGameObject);
			return newGameObject;
		}

	private:

		// Array of intialised game objects.
		std::vector<Pointer<GameObject>> objects;

	};
}