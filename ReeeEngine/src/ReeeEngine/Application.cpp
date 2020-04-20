#include "Application.h"
#include "ReeeLog.h"
#include <iostream>

namespace ReeeEngine
{
	// Main window created here.
	Application::Application() : engineWindow(800, 600, "Reee Editor")
	{
		//...
	}

	Application::~Application()
	{
		//...
	}

	int Application::Start()
	{
		// Catch any exceptions while updating the engine.
		try
		{
			// Run begin first and once.
			Init();

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
		catch (const ReeeException& e)
		{
			MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		}
		catch (const std::exception& e)
		{
			MessageBox(nullptr, e.what(), "Default Exception", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (...)
		{
			MessageBox(nullptr, "No information found", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
		}	
	}

	void Application::Init()
	{
		// Initalise the logging system.
		ReeeLog::InitaliseLogging();

		REEE_LOG(Log, "Intialised Engine....");
	}

	void Application::Tick()
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
