#pragma once
#include "../Globals.h"
#include "../Rendering/Graphics.h"
#include "../Delegates/Delegate.h"
#include "WindowsInput.h"
#include <optional>
#include <memory>

namespace ReeeEngine
{
	/* Helper window macros for throwing exceptions from the window class. */
    #define WINDOW_THROW_EXCEPT(...) { REEE_LOG(Error, "Window Error: ", __VA_ARGS__); __debugbreak(); }
	#define WINDOW_EXCEPT(result, ...) if(!result) { WINDOW_THROW_EXCEPT(__VA_ARGS__); }

	/* Class to manage registration and cleanup for a given window.
	 * Also will handle any input and messages passed to the window and call sub functions for engine input, error messages etc. */
	class REEE_API Window
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

		/* Begin and end rendering for the window. */
		void BeginFrame();
		void EndFrame();

		/* Window functions. NOTE: Set the windows title. */
		void SetTitle(const std::string& newTitle);

		/* Set the initial delegate callback on window creation. */
		void SetDelegateBroadcastEvent(const std::function<void(Delegate&)>& callback);

		/* Window message processor. */
		static std::optional<int> DispatchMessages() noexcept;

		/* Returns pointer to graphics. */
		Graphics& GetGraphics();

		/* Getters for the window height and width. */
		int GetWidth();
		int GetHeight();

		/* Return the windows hWnd. */
		HWND GetHwnd();

		/* Resize the given window. */
		void ForceResizeWindow(int newWidth = 0, int newHeight = 0);

		/* Cursor functionality for enabling and disabling over the window for game based raw mouse input... */
		void EnableCursor() noexcept;
		void DisableCursor() noexcept;
		bool CursorEnabled() const noexcept;
		bool CursorHidden() const noexcept;
		void ConfineCursor() noexcept;
		void FreeCursor() noexcept;
		void HideCursor() noexcept;
		void ShowCursor() noexcept;

	private:

		/* Create callback function for windows message events. */
		std::function<void(Delegate&)> BroadcastDel;

		/* Message handling functions to perform actions based of user input with the window class. */
		static LRESULT CALLBACK HandleMessageEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMessageRuntime(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		/* Window variables. */
		int currWidth;
		int currHeight;
		HWND hWnd;
		Refference<Graphics> graphics;

		/* Saved window's settings for adjusting rect at runtime. */
		long windowSettings;

		/* Extra input variables. */
		int keyRepeatCount = 0;
		bool cursorEnabled = true;
		bool cursorHidden = false;
		std::vector<BYTE> rawBuffer;

	public:

		/* Public input manager/receiver. */
		WindowsInput input;
	};
}