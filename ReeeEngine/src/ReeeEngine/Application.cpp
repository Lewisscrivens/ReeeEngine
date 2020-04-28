#include "Application.h"
#include "ReeeLog.h"
#include "Rendering/Renderables/RenderableMesh.h"
#include "Rendering/Renderables/Shapes/Box.h"
#include "Rendering/Renderables/Shapes/Sphere.h"
#include <iostream>

namespace ReeeEngine
{
	// Main window created here.
	Application::Application()
	{
		// Create engine window.
		engineWindow = CreateReff<Window>(1280, 720, "Reee Editor");
		engineWindow->SetDelegateCallback(BIND_DELEGATE(Application::OnDelegate));

		// Add a box and setup the projection matrix.
		renderables.push_back(CreateReff<Box>(engineWindow->GetGraphics(), Vector3D(1.0f, 10.0f, 0.0f), Vector3D(0.0f), Vector3D(5.0f)));
		renderables.push_back(CreateReff<Sphere>(engineWindow->GetGraphics(), 1.0f, Vector3D(1.0f, 0.0, 0.0f)));
		engineWindow->GetGraphics().SetProjectionMatrix(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	}

	Application::~Application()
	{
		//...


	}

	int Application::Start()
	{
		// Catch any unhandled exceptions from the engine while running.
		try
		{
			// Run begin first and once.
			Init();

			// Enter infinite loop.
			while (true)
			{
				// Update message processing.
				if (const std::optional<int> optionalReturn = engineWindow->DispatchMessages())
				{
					// If the optional function returned anything return for update.
					return *optionalReturn;
				}

				// Update frame.
				Tick();
			}
		}
		catch (const std::exception& e)
		{
			MessageBox(nullptr, e.what(), "Default Exception", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (...)
		{
			MessageBox(nullptr, "No information found", "Unhandled Exception", MB_OK | MB_ICONEXCLAMATION);
		}	

		// Return failed if gets to this point.
		return -1;
	}

	void Application::Init()
	{
		REEE_LOG(Log, "Intialised Engine....");


	}

	void Application::Tick()
	{
		// Create delta time.
		const auto deltaTime = timer.GetDeltaTime();

		// Clear render buffer before each render call.
		engineWindow->GetGraphics().ClearRenderBuffer();

		// Tick and render each renderable object active in the engine window.
		for (auto& renderable : renderables)
		{
			renderable->Tick(deltaTime);
			renderable->Render(engineWindow->GetGraphics());
		}

		// Present the frame to the window after dispatching input messages.
		engineWindow->GetGraphics().EndFrame();
	}

	void Application::OnDelegate(Delegate& del)
	{
		DelegateDispatcher dispatcher(del);
		dispatcher.Dispatch<WindowResizedDelegate>(BIND_DELEGATE(Application::OnWindowResized));
		dispatcher.Dispatch<WindowClosedDelegate>(BIND_DELEGATE(Application::OnWindowClosed));

		//...
	}

	bool Application::OnWindowResized(WindowResizedDelegate& del)
	{
		REEE_LOG(Log, "{0}", del.ToString());
		return false;
	}

	bool Application::OnWindowClosed(WindowClosedDelegate& del)
	{
		REEE_LOG(Log, "Engine closed.");
		PostQuitMessage(0);
		return true;
	}
}
