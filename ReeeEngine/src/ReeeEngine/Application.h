#pragma once
#include "Timer.h"
#include "Windows/Window.h"
#include "Delegates/WindowDelegates.h"
#include "Delegates/InputDelegates.h"
#include "Module/ModuleManager.h"
#include "Module/UserInterfaceModule.h"
#include "OpenCV/OpenCVInput.h"

namespace ReeeEngine
{
	class REEE_API Application
	{
	public:

		/* Default app constructor and destructor. */
		Application();
		~Application();

		/* Starting function with while Loop which loops until application is exited through error or user. */
		virtual int Start();

		/* Ran on engine start for initialization of certain engine components. */
		virtual void Init();

		/* Ran when a delegate is called so it can be handled. */
		void OnDelegate(Delegate& del);

		/* Helper functions for adding modules to the engine and initializing them. */
		void AddModule(Module* newModule);
		void AddModuleFront(Module* newModule);

		/* Ran when the window is resized. */
		bool OnWindowResized(WindowResizedDelegate& del);

		/* Ran when the window is closed. */
		bool OnWindowClosed(WindowClosedDelegate& del);

		/* Key pressed delegate event. */
		bool OnKeyPressed(KeyPressedDelegate& del);

		/* Window getter. */
		Window& GetWindow() { return *engineWindow; };

		/* Get a pointer to the world. */
		static class World* GetWorld();

		/* Static getters for the application for access by subclasses. */
		static Application& GetEngine() { return *app; }

		/* Static getter for opencv input. */
		static Pointer<OpenCVInput>& GetOpenCVInput() { return app->visualInput; }

		/* Is this application running. */
		bool IsAppRunning() { return appRunning; }

	protected:

		/* Frame. */
		virtual void Tick(float deltaTime);

	private:

		/* Static pointer to this app. */
		static Application* app;

		/* Reference to the main window class. */
		Refference<Window> engineWindow;

		/* Refference to the user interface module. */
		UserInterfaceModule* userInterface;

		/* The engine world that will load levels from .txt files when fully implemented. */
		World* world;

		/* Timer for calculating engine time and delta time. */
		Timer timer;

		/* Engine module manager. */
		ModuleManager modules;

		/* Visual input manager. */
		Pointer<OpenCVInput> visualInput;
		std::thread* initOpenCVThread;

		/* Information about the state of the application. */
		bool appRunning = true;
		bool gamePaused = false;
		bool minimised = false;
		float deltaTime = 0.0f;
		float framerate = 0.0f;
	};

	/* Define in the sub application. */
	Application* CreateApp();
}
