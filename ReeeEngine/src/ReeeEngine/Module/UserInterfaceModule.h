#pragma once
#include "Module.h"
#include "../Globals.h"
#include "../Delegates/InputDelegates.h"

namespace ReeeEngine
{
	class REEE_API UserInterfaceModule : public Module
	{
	public:

		/* Module constructor/destructor. */
		UserInterfaceModule();
		~UserInterfaceModule() = default;

		/* Ran to initalise the UserInterface module using Imgui. */
		virtual void InitModule() override;

		/* Ran to shut down imgui components. */
		virtual void ShutdownModule() override;

		/* Tick function. */
		virtual void Tick(float deltaTime) override;

		/* Passed delegate events to this module if they have not been
		 * handled by another module of the engine. */
		virtual void OnDelegate(Delegate& del) override;

		/* Imgui rendering functions. */
		void BeginFrame();
		virtual void OnImGuiRender() override;
		void EndFrame();	

	private:

		/* Static pointer to the app. */
		class Application& appPointer;

		// Input event handlers.
		bool MouseButtonPressed(MousePressedDelegate& del);
		bool MouseButtonReleased(MouseReleasedDelegate& del);
		bool MouseMoved(MouseMovedDelegate& del);
		bool MouseScrolled(MouseScrolledDelegate& del);
		bool KeyPressed(KeyPressedDelegate& del);
		bool KeyReleased(KeyReleasedDelegate& del);
		bool CharPressed(CharPressedDelegate& del);
	};
}