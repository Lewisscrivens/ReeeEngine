#include "World.h"
#include "Core/GameObject.h"

namespace ReeeEngine
{
	World::World()
	{
		//...
	}

	World::~World()
	{
		// Destroy all objects loaded into the world.
		objects.clear();
	}

	void World::LevelStart()
	{
		// For each loaded object run level start.
		for (auto& obj : objects)
		{
			obj->LevelStart();
		}
	}

	void World::Tick(float deltaTime)
	{
		// For each loaded object tick.
		for (auto& obj : objects)
		{
			obj->Tick(deltaTime);
		}
	}
}

