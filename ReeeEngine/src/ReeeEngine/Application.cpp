#include "Application.h"
#include "ReeeLog.h"
#include "Rendering/Renderables/RenderableMesh.h"
#include "Rendering/Renderables/Shapes/Sphere.h"
#include "Rendering/Renderables/Mesh.h"
#include "Rendering/Lights/PointLight.h"
#include "World/World.h"
#include "World/Components/CameraComponent.h"
#include "Profiling/DebugTimer.h"
#include <thread>

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
		if (initOpenCVThread)
		{
			initOpenCVThread->join();
			delete initOpenCVThread;
		}
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

				// Create delta time.
				deltaTime = timer.GetDeltaTime();
				framerate = 1.0f / deltaTime;

				// Update frame.
				Tick(deltaTime);
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
		// Initalise the visual input manager for opencv on a separate thread to prevent opening delay...
		// Best tracking types are either MOSSE for speed and KCF for a balance between speed and accuracy.
		initOpenCVThread = new std::thread([this]() mutable { visualInput = CreatePointer<OpenCVInput>(TrackType::MOSSE); });

		// Create and initalise the world.
		world = new World();
		world->LevelStart();

		// Create engine window.
		engineWindow = CreateReff<Window>(1280, 720, "Reee Editor");
		auto delegateDispatcher = BIND_DELEGATE(Application::OnDelegate);
		engineWindow->SetDelegateBroadcastEvent(delegateDispatcher);
		pointLight = new PointLight(engineWindow->GetGraphics());

		// Initalise the imgui module.
		userInterface = new UserInterfaceModule();
		AddModuleFront(userInterface);

		// Test model loading from mesh.
		renderables.push_back(CreateReff<Mesh>(engineWindow->GetGraphics(), "../Assets/PlayerCar", Vector3D(0.0f, -2.0f, -2.0f), Rotator(0.0f), Vector3D(1.0f), 1.0f));
		renderables.push_back(CreateReff<Mesh>(engineWindow->GetGraphics(), "../Assets/PoliceCar", Vector3D(0.0f, -2.0f, 2.0f), Rotator(0.0f), Vector3D(1.0f), 1.0f));
		renderables.push_back(CreateReff<Mesh>(engineWindow->GetGraphics(), "../Assets/skybox", Vector3D(0.0f, 10.0f, 0.0f), Rotator(0.0f), Vector3D(-1000.0f), 1.0f, true));

		// Log initialization...
		REEE_LOG(Log, "Intialised Engine....");
	}

	void Application::Tick(float deltaTime)
	{
		// Begin rendering window frame.
		engineWindow->BeginFrame();

		// Tick the world and objects within it.
		if (!gamePaused) world->Tick(deltaTime);

		// Update visual input device once it has been created and intialised.
		if (visualInput && visualInput->IsInitialised())
		{
			visualInput->Update();
		}
		
		// Bind point light information to the pipeline to be accessed by renderables during rendering/binding of there
		// individual pixel shaders.
		pointLight->Add(engineWindow->GetGraphics(), world->GetActiveCamera().GetViewMatrix());
		
		// Tick and render each renderable object active in the engine window.
		for (auto& renderable : renderables)
		{
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
		dispatcher.Dispatch<KeyPressedDelegate>(BIND_DELEGATE(Application::OnKeyPressed));

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
		deltaTime = timer.GetDeltaTime();
		Tick(deltaTime);

		// Return false as we want the windows resized delegate to be 
		// handled by all engine modules not just the first one to handle it.
		return false;
	}

	bool Application::OnWindowClosed(WindowClosedDelegate& del)
	{
		// Close the app.
		REEE_LOG(Log, "Engine closed.");
		appRunning = false;
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedDelegate& del)
	{
		// When space is pressed reset open cv tracking...
		if (del.GetKeyCode() == KEY_SPACE)
		{
			if (visualInput && visualInput->IsInitialised())
			{
				visualInput->ReinitTracking(5.0f);
			}
		}

		// Handle in other areas also.
		return false;
	}

	World* Application::GetWorld()
	{
		return app->world;
	}
}
