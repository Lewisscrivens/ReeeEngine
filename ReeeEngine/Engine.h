#pragma once
#include "Window.h"
#include "Timer.h"

namespace ReeeEngine
{
	/* Engine macros. */
	#define DEBUG_ENABLED 1;

	/* Main application class to handle updating modules within the engine. */
	class Engine
	{
	public:

		/* Constructor. NOTE: Creates mainWindow instance and timer instance. */
		Engine();

		/* Starting function with while Loop which loops until application is exited through error or user. */
		int Start();

		/* Ran on engine start for initialisation of certain engine components. */
		void Begin();

	private:

		/* Frame. */
		void Tick();

	private:

		/* Reference to the main window class. */
		Window engineWindow;
		Timer timer;
	};
}

