#pragma once
#include "../Globals.h"
#include "../Delegates/Delegate.h"

namespace ReeeEngine
{
	/* Base class for a module of the engine. */
	class REEE_API Module
	{
	public:

		/* Initalise the module. */
		Module(const std::string& name = "EngineModule") { moduleName = name; };
		virtual ~Module() = default;	

		/* Ran when the module has been created to initalise module submodules/objects. */
		virtual void InitModule() = 0;

		/* Ran when the application is being shutdown. */
		virtual void ShutdownModule() = 0;

		/* Tick function ran per frame of application. */
		virtual void Tick(float deltaTime) = 0;

		/* When a delegate is ran it is passed to this module if not previously marked 
		 * handled by the application or another module. */
		virtual void OnDelegate(Delegate& del) = 0;

		/* Render Imgui components for each module. */
		virtual void OnImGuiRender() = 0;

		/* Return this modules name. */
		const std::string& GetModuleName() const { return moduleName; };

	protected:

		// This modules name in-case needed in profiler.
		std::string moduleName;
	};
}
