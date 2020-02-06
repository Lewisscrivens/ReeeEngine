#include "Engine.h"

namespace ReeeEngine
{
	// Main window created here.
	Engine::Engine() : mainWindow(800, 600, "Reee Engine") {}

	void Engine::Begin()
	{
		// Anything that should be ran before update goes here...

	}

	int Engine::Start()
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
		// Change background color over time.
		const float newBackgroundCol = std::abs((float)(sin(timer.GetTime()) / 2.0f + 0.5f));
		mainWindow.GetGraphics().ClearRenderBuffer(newBackgroundCol, newBackgroundCol - (newBackgroundCol / 2.0f), 1.0f);

		// Draw cube each frame using the graphics class.
		mainWindow.GetGraphics().DrawCube(timer.GetTime(), mainWindow.input.GetMousePosition().X / 400.0f - 1.0f, -mainWindow.input.GetMousePosition().Y / 300.0f + 1.0f);

		// Present the frame to the window after dispatching input messages.
		mainWindow.GetGraphics().EndFrame();
	}
}