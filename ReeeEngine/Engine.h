#pragma once
#include "Window.h"
#include "Timer.h"

/* Engine macros. */
#define DEBUG_ENABLED 1;

/* Main application class to handle updating modules within the engine. */
class Engine
{
public:

	/* Constructor. NOTE: Creates mainWindow instance and timer instance. */
	Engine();

	/* Ran on engine start. */
	void Begin();

	/* Looped until application is exited. */
	int Update();

private:

	/* Frame. */
	void Tick();

private:

	/* Reference to the main window class. */
	Window mainWindow;
	Timer timer;
};

