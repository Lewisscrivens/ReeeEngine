#include "Application.h"
#include "ReeeLog.h"
#include "Rendering/Renderables/RenderableMesh.h"
#include "Rendering/Renderables/Shapes/Box.h"
#include "Rendering/Renderables/Shapes/Sphere.h"
#include "Profiling/DebugTimer.h"

namespace ReeeEngine
{
	// Main window created here.
	Application::Application()
	{
		// Create engine window.
		engineWindow = CreateReff<Window>(1280, 720, "Reee Editor");
		engineWindow->SetDelegateCallbackEvent(BIND_DELEGATE(Application::OnDelegate));

		// Add a box and setup the projection matrix.
		renderables.push_back(CreateReff<Box>(engineWindow->GetGraphics(), Vector3D(1.0f, 10.0f, 0.0f), Vector3D(0.0f), Vector3D(5.0f)));
		renderables.push_back(CreateReff<Sphere>(engineWindow->GetGraphics(), 1.0f, Vector3D(1.0f, 0.0, 0.0f), Vector3D(0.0f), Vector3D(-50.0f)));
		
		// Set default projection matrix.
		engineWindow->GetGraphics().SetProjectionMatrix();
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
			while (appRunning)
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

		// Update tick event for each module in order.
		for (Module* module : modules)
		{
			module->Tick(deltaTime);
		}
	}

	void Application::OnDelegate(Delegate& del)
	{
		DelegateDispatcher dispatcher(del);
		dispatcher.Dispatch<WindowResizedDelegate>(BIND_DELEGATE(Application::OnWindowResized));
		dispatcher.Dispatch<WindowClosedDelegate>(BIND_DELEGATE(Application::OnWindowClosed));

		// Send delegate events to each module within the engine in a given update order.
		for (auto it = modules.end(); it != modules.begin();)
		{
			(*--it)->OnDelegate(del);
			if (del.handled) break;
		}
	}

	void Application::AddModule(Module* newModule)
	{
		modules.AddModule(newModule);
		newModule->InitModule();
	}

	void Application::AddModuleFront(Module* newModule)
	{
		modules.AddModuleFront(newModule);
		newModule->InitModule();
	}

	bool Application::OnWindowResized(WindowResizedDelegate& del)
	{
		// Update if the window has been minimised.
		if (del.GetNewHeight() == 0 || del.GetNewWidth() == 0)
		{
			minimised = true;
			return true;
		}
		else if (minimised)
		{
			minimised = false;
		}

		// Update render target size's in graphics.
		engineWindow->GetGraphics().ResizeRenderTargets(del.GetNewWidth(), del.GetNewHeight());

		// Update application while its being resized.
		// NOTE: Stuck in dispach message so has to be updates this way while being resized.
		Tick();

		// Return false as we want the windows resized delegate to be 
		// handled by all engine modules not just the first one to handle it.
		return false;
	}

	bool Application::OnWindowClosed(WindowClosedDelegate& del)
	{
		REEE_LOG(Log, "Engine closed.");
		appRunning = false;
		return true;
	}
}
