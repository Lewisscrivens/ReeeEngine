#include "Engine.h"

// Main window initialized here.
Engine::Engine() : mainWindow(800, 600, "Reee Engine") {}

void Engine::Begin()
{
	// Anything that should be ran before update goes here...
}

int Engine::Update()
{
	// Run begin first and once.
	Begin();

	// Enter infinite loop.
	while (true)
	{
		// Update message processing.
		if (const std::optional<int> optionalReturn = mainWindow.DispatchMessages())
		{
			// If the optional function returned anything return for update.
			return *optionalReturn;
		}

		// Update frame.
		Tick();
	}
}

void Engine::Tick()
{
	// Present the frame to the window after dispatching input messages.
	mainWindow.GetGraphics().EndFrame();
}
