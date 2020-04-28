#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Windows/Window.h"
#include "Delegates/WindowDelegates.h"

namespace ReeeEngine
{
	class REEE_API Application
	{
	public:

		/* Default app constructor and destructor. */
		Application();
		~Application();

		/* Starting function with while Loop which loops until application is exited through error or user. */
		virtual int Start();

		/* Ran on engine start for initialization of certain engine components. */
		virtual void Init();

		/* Ran when a delegate is called so it can be handled. */
		void OnDelegate(Delegate& del);

		/* Ran when the window is resized. */
		bool OnWindowResized(WindowResizedDelegate& del);

		/* Ran when the window is closed. */
		bool OnWindowClosed(WindowClosedDelegate& del);

	protected:

		/* Frame. */
		virtual void Tick();

	private:

		/* Reference to the main window class. */
		Refference<Window> engineWindow;

		/* Timer for calculating engine time and delta time. */
		Timer timer;

		/* Reference to all renderable's within the current application to be rendered by graphics. */
		std::vector<Refference<class RenderableMesh>> renderables;
	};

	/* Define in the sub application. */
	Application* CreateApp();
}
