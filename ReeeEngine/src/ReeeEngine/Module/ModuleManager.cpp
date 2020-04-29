#include "ModuleManager.h"

namespace ReeeEngine
{
	ModuleManager::~ModuleManager()
	{
		// Shutdown active modules on application end.
		for (Module* module : engineModules)
		{
			module->ShutdownModule();
			delete module;
		}
	}

	void ModuleManager::AddModule(Module* newModule)
	{
		engineModules.emplace(engineModules.begin() + lastInsertIndex, newModule);
		lastInsertIndex++;
	}

	void ModuleManager::AddModuleFront(Module* newModule)
	{
		engineModules.emplace_back(newModule);
	}
}