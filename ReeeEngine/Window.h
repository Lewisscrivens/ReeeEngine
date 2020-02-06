#pragma once
#include "ReeeWin.h"
#include "ReeeException.h"
#include "WindowsInput.h"
#include "Graphics.h"
#include "WindowException.h"
#include <optional>
#include <memory>

namespace ReeeEngine
{
	/* Helper window macros for throwing exceptions from the window class. */
	#define WINDOW_EXCEPT(hr) WindowException::HrException(__LINE__, __FILE__, hr)
	#define WINDOW_LAST_EXCEPT() WindowException::HrException(__LINE__, __FILE__, GetLastError())
	#define WINDOW_NOGRAPHICS_EXCEPT() WindowException::NoGraphicsException(__LINE__, __FILE__)

	/* Class to manage registration and cleanup for a given window.
	 * Also will handle any input and messages passed to the window and call sub functions for engine input, error messages etc. */
	class Window
	{
	private:

		/* Static window class to hold initialized windows variables. */
		class WindowClass
		{
		private:

			/* Constructor for registration and destructor for un-registration. */
			WindowClass() noexcept;
			~WindowClass();

			/* Handle window class operators. */
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator = (const WindowClass&) = delete;

			/* Create variables for this class for the name and reference to the hInstance for the window. */
			static constexpr const char* windowClassName = "Reee Engine";
			static WindowClass windowClass;
			HINSTANCE hInstance;

		public:

			/* Return the windowClassName. */
			static const char* GetName() noexcept;

			/* Return the hInstance for this window. */
			static HINSTANCE GetInstance() noexcept;

		};

	public:

		/* Constructor and destructor. */
		Window(int width, int height, const char* name);
		~Window();

		/* Operator overrides. */
		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;

		/* Window functions. NOTE: Set the windows title. */
		void SetTitle(const std::string& newTitle);

		/* Window message processor. */
		static std::optional<int> DispatchMessages() noexcept;

		/* Returns pointer to graphics. */
		Graphics& GetGraphics();

	private:

		/* Message handling functions to perform actions based of user input with the window class. */
		static LRESULT CALLBACK HandleMessageEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMessageRuntime(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		/* Window variables. */
		int width;
		int height;
		HWND hWnd;
		std::unique_ptr<Graphics> graphics;

	public:

		/* Public input manager/receiver. */
		WindowsInput input;

		/* Return the windows hWnd. */
		HWND GetHwnd();
	};
}