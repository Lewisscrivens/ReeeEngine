#include "Engine.h"

namespace ReeeEngine
{
	// Main window created here.
	Engine::Engine() : engineWindow(800, 600, "Reee Editor") {}

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
			if (const std::optional<int> optionalReturn = engineWindow.DispatchMessages())
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
		engineWindow.GetGraphics().ClearRenderBuffer(newBackgroundCol, newBackgroundCol - (newBackgroundCol / 2.0f), 1.0f);

		// Draw cube each frame using the graphics class.
		engineWindow.GetGraphics().DrawCube(timer.GetTime(), engineWindow.input.GetMousePosition().X / 400.0f - 1.0f, -engineWindow.input.GetMousePosition().Y / 300.0f + 1.0f);

		// Present the frame to the window after dispatching input messages.
		engineWindow.GetGraphics().EndFrame();
	}
}