#include "Window.h"
#include <sstream>
#include "../Delegates/WindowDelegates.h"

namespace ReeeEngine
{
	Window::WindowClass Window::WindowClass::windowClass;

	Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr))
	{
		// Configure the window and register it.
		WNDCLASSEX windowsClass = { 0 };
		windowsClass.cbSize = sizeof(windowsClass);
		windowsClass.style = CS_OWNDC;
		windowsClass.lpfnWndProc = HandleMessageEntry; // Set windowProcedure function for this window.
		windowsClass.cbClsExtra = 0;
		windowsClass.cbWndExtra = 0;
		windowsClass.hInstance = GetInstance();
		HICON windowIcon = (HICON)LoadImage(hInstance, "../bin/Debug-x64/ReeeEngine/ReeeIcon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
		windowsClass.hIcon = windowIcon;
		windowsClass.hCursor = nullptr;
		windowsClass.hbrBackground = nullptr;
		windowsClass.lpszMenuName = nullptr;
		windowsClass.lpszClassName = GetName();
		windowsClass.hIconSm = windowIcon;
		RegisterClassEx(&windowsClass);
	}

	Window::WindowClass::~WindowClass()
	{
		UnregisterClass(windowClassName, GetInstance());
	}

	const char* Window::WindowClass::GetName() noexcept
	{
		return windowClassName;
	}

	HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return windowClass.hInstance;
	}

	Window::Window(int width, int height, const char* name) : width(width), height(height)
	{
		// Calculate the size of the window for current client region.
		RECT windowRect;
		windowRect.left = 100;
		windowRect.right = width + windowRect.left;
		windowRect.top = 100;
		windowRect.bottom = height + windowRect.top;
		bool result = AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
		WINDOW_EXCEPT(result, "Failed to adjust the windows size in the window class.");

		// Add default settings to the window.
		auto windowSettings = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;

		// Create the window and get the hWnd value.
		hWnd = CreateWindow(WindowClass::GetName(), name, windowSettings, CW_USEDEFAULT, CW_USEDEFAULT, 
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, WindowClass::GetInstance(), this);

		// Check if creating the window failed.
		if (hWnd == nullptr) WINDOW_THROW_EXCEPT("Failed to create a new window!!!");

		// Show the window.
		ShowWindow(hWnd, SW_SHOWDEFAULT);

		// Create graphics object.
		graphics = CreateReff<Graphics>(hWnd, width, height);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		// Set new title and if it failed throw exception.
		if (SetWindowText(hWnd, newTitle.c_str()) == 0) WINDOW_THROW_EXCEPT("Failed to set the windows title text.");
	}

	void Window::SetDelegateCallback(const std::function<void(Delegate&)>& callback)
	{
		callbackDel = callback;
	}

	std::optional<int> Window::DispatchMessages() noexcept
	{
		// Perform and dispatch message events from the window.
		MSG message;
		while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))// Hwnd was where nullptr is now...
		{
			// Return the post quit message.
			if (message.message == WM_QUIT) return message.wParam;
	
			// Translate then dispatch message.
			TranslateMessage(&message);
			DispatchMessage(&message); // Pass message to the windows processor for this window.
		}
	
		// Return nothing as the return is optional type.
		return {};
	}

	Graphics& Window::GetGraphics()
	{
		// If there is no graphics found throw no graphics exception otherwise return the graphics.
		if (!graphics) WINDOW_THROW_EXCEPT("Failed to get the graphics device from the window.");
		return *graphics;
	}

	Window::~Window()
	{
		DestroyWindow(hWnd);// Window destructor.
	}

	HWND Window::GetHwnd()
	{
		return hWnd;
	}

	LRESULT CALLBACK Window::HandleMessageEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// Just after creation / on creation store pointers to the windowClass in the windows API user data.
		if (msg == WM_NCCREATE)
		{
			// Get pointer to window class from lPraram.
			const CREATESTRUCTW* const dataPointer = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* CONST windowPointer = static_cast<Window*>(dataPointer->lpCreateParams);

			// Store the pointer within the WindowAPI data set for the windowClass and runtime message handling function.
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPointer));

			// Return handled message from found window pointer.
			return windowPointer->HandleMessage(hWnd, msg, wParam, lParam);
		}

		// Send message to the main message handler to obtain the window class from Windows API user data.
		return HandleMessageRuntime(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK Window::HandleMessageRuntime(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// Get pointer to window class from the Windows API user data.
		Window* CONST windowPointer = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// Return the handled message from the handle message function for the window.
		return windowPointer->HandleMessage(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// Create empty delegate to be handled using switch.
		Delegate* del = nullptr;

		// Handle messages being sent from the window class.
		switch (msg)
		{
		// On application closed.
		case WM_CLOSE:
		{
			WindowClosedDelegate closedDel;
			del = &closedDel;
			break;
		}	
		// On window resize.
		case WM_SIZE:
		{
			WindowResizedDelegate resizedDel(LOWORD(lParam), HIWORD(lParam));
			del = &resizedDel;
			break;
		}

	/* KEYBOARD INPUT UPDATE MESSAGES. */
	#if KEYBOARD_ENABLED

		// On window focus lost clear the current keySet to prevent ghost key presses.
		case WM_KILLFOCUS:
			input.ClearKeySet();
		break;

		// Pass any keyboard events to the keyboard class for this window.
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			// Only run key pressed event repeatedly if autorepeat is enabled or its the first keyboard press for the key.
			if (!(lParam & 0x40000000) || input.IsAutorepeatEnabled()) input.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			input.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

		case WM_CHAR:
			input.OnCharPressed(static_cast<unsigned char>(wParam));
		break;

	#endif
	/* MOUSE INPUT UPDATE MESSAGES. */
	#if MOUSE_ENABLED

		// Capture mouse movement inside and outside of the window if the mouse buttons are held.
		case WM_MOUSEMOVE:
		{
			// Get the mouse position within the window.
			POINTS mousePos = MAKEPOINTS(lParam);

			// If the mouse is within the window update mouse move as normal.
			if (mousePos.x >= 0 && mousePos.x < width && mousePos.y >= 0 && mousePos.y < height)
			{
				input.OnMouseMove(mousePos.x, mousePos.y);

				// If the mouse is registered as not being in the window toggle it.
				if (!input.IsMouseInWindow())
				{
					SetCapture(hWnd);
					input.OnMouseInside();
				}
			}
			// If the mouse has left the window use mouse capture to track movement.
			else
			{
				// If either the left or right mouse button is currently being held down update mouse movement while outside of the window.
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					input.OnMouseMove(mousePos.x, mousePos.y);
				}
				// Otherwise end mouse capture and set the mouse as outside of the window.
				else
				{
					ReleaseCapture();
					input.OnMouseOutside();
				}
			}
		}
		break;
		case WM_LBUTTONDOWN:
			input.OnMousePressed(WindowsInput::EMouseButton::Left);
			break;
		case WM_RBUTTONDOWN:
			input.OnMousePressed(WindowsInput::EMouseButton::Right);
			break;
		case WM_MBUTTONDOWN:
			input.OnMousePressed(WindowsInput::EMouseButton::Middle);
			break;
		case WM_LBUTTONUP:
			input.OnMouseReleased(WindowsInput::EMouseButton::Left);
			break;
		case WM_RBUTTONUP:
			input.OnMouseReleased(WindowsInput::EMouseButton::Right);
			break;
		case WM_MBUTTONUP:
			input.OnMouseReleased(WindowsInput::EMouseButton::Middle);
			break;
		case WM_MOUSEWHEEL:
		{
			// Pass and handle wheel scrolling into the input class.
			const int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			input.OnMouseWheelDelta(wheelDelta);
			break;
		}	
	#endif
		}

		// If there are any delegates waiting to be handled call them.
		if (del && callbackDel)
		{
			callbackDel(*del);
		}

		// Handle any messages not being handled in this function.
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}