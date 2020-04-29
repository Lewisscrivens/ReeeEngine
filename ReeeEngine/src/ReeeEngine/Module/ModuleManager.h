#pragma once
#include "Module.h"
#include "../Globals.h"

namespace ReeeEngine
{
	/* Management class for creating and shutting down modules for ReeeEngine. */
	class REEE_API ModuleManager
	{
	public:

		/* Default constructor. */
		ModuleManager() = default;

		/* Destructor to shutdown any created engine modules on application shutdown. */
		~ModuleManager();

		/* Add a new module to be updated at the end of the module list. */
		void AddModule(Module* newModule);

		/* Add a new module to be updated at the front of the module list.
		 * NOTE: Anything added here will be rendered over any modules inserted towards the end. */
		void AddModuleFront(Module* newModule);

		// For now theres no way of removing modules once added as I don't 
		// intend to change them during runtime.

		// Iterator for iterating through the modules using this class.
		std::vector<Module*>::iterator begin() { return engineModules.begin(); };
		std::vector<Module*>::iterator end() { return engineModules.end(); };

	private:

		// Current list of active engine modules.
		std::vector<Module*> engineModules;
		int lastInsertIndex = 0;
	};
}



