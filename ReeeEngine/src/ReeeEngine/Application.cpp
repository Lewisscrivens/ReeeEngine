#include "Application.h"
#include "ReeeLog.h"
#include "Rendering/Renderables/RenderableMesh.h"
#include "Rendering/Renderables/Shapes/Box.h"
#include "Rendering/Renderables/Shapes/Sphere.h"
#include "World/World.h"
#include "World/Components/CameraComponent.h"

namespace ReeeEngine
{
	// Declare static app.
	Application* Application::app = nullptr;

	// Main window created here.
	Application::Application()
	{
		// Set static app pointer.
		app = this;
	}

	Application::~Application()
	{
		// Shutdown any engine management classes.

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
		// Create and initalise the world.
		world = new World();
		world->LevelStart();

		// Create engine window.
		engineWindow = CreateReff<Window>(1280, 720, "Reee Editor");
		auto delegateDispatcher = BIND_DELEGATE(Application::OnDelegate);
		engineWindow->SetDelegateBroadcastEvent(delegateDispatcher);

		// Initalise the imgui module.
		userInterface = new UserInterfaceModule();
		AddModuleFront(userInterface);

		// Add a box and setup the projection matrix.
		// NOTE: Testing code....
		renderables.push_back(CreateReff<Box>(engineWindow->GetGraphics(), Vector3D(1.0f, 10.0f, 0.0f), Rotator(0.0f), Vector3D(5.0f)));
		renderables.push_back(CreateReff<Sphere>(engineWindow->GetGraphics(), 1.0f, Vector3D(1.0f, 0.0, 0.0f), Rotator(0.0f), Vector3D(5.0f)));

		// Log initialization...
		REEE_LOG(Log, "Intialised Engine....");
	}

	void Application::Tick()
	{
		// Create delta time.
		const auto deltaTime = timer.GetDeltaTime();

		// Begin rendering window frame.
		engineWindow->BeginFrame();

		// Tick the world and objects within it.
		if (!gamePaused)
		{
			world->Tick(deltaTime);
		}

		// Tick and render each renderable object active in the engine window.
		for (auto& renderable : renderables)
		{
			renderable->Tick(deltaTime);
			renderable->Render(engineWindow->GetGraphics());
		}

		// Update user interface module and each other module. Also run tick events.
		userInterface->BeginFrame();
		for (Module* module : modules)
		{
			module->OnImGuiRender();
			module->Tick(deltaTime);
		}
		userInterface->EndFrame();

		// End rendering window frame.
		engineWindow->EndFrame();
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

		// Update active cameras FOV from the new width and height.
		world->GetActiveCamera().SetWindowSize(del.GetNewWidth(), del.GetNewHeight());

		// Update ticking function so the rendering updates while scaling....
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

	World* Application::GetWorld()
	{
		return app->world;
	}
}
