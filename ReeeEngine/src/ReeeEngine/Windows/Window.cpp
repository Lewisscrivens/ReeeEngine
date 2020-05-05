#include "Window.h"
#include <sstream>
#include <vector>
#include "../Delegates/WindowDelegates.h"
#include "../Delegates/InputDelegates.h"

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
		windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);
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

	Window::Window(int width, int height, const char* name) : currWidth(width), currHeight(height)
	{
		// Calculate the size of the window for current client region.
		RECT windowRect;
		windowRect.left = 100;
		windowRect.right = width + windowRect.left;
		windowRect.top = 100;
		windowRect.bottom = height + windowRect.top;

		// Add default settings to the window.
	    windowSettings = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
		bool result = AdjustWindowRect(&windowRect, windowSettings, TRUE);
		WINDOW_EXCEPT(result, "Failed to adjust the windows size of the window rect.");

		// Create the window and get the hWnd value.
		hWnd = CreateWindowEx(0, WindowClass::GetName(), name, windowSettings, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, 
			windowRect.bottom - windowRect.top, nullptr, nullptr, WindowClass::GetInstance(), this);

		// Check if creating the window failed.
		if (hWnd == nullptr) WINDOW_THROW_EXCEPT("Failed to create a new window!!!");

		// Show the window.
		ShowWindow(hWnd, SW_SHOWDEFAULT);

		// Create graphics object.
		graphics = CreateReff<Graphics>(hWnd, width, height);

		// Register mouse raw input device for game input (Prevents mouse acceleration etc.)
		RAWINPUTDEVICE rawInput;
		rawInput.usUsagePage = 0x01;
		rawInput.usUsage = 0x02;
		rawInput.dwFlags = 0;
		rawInput.hwndTarget = nullptr;
		result = RegisterRawInputDevices(&rawInput, 1, sizeof(rawInput));
		WINDOW_EXCEPT(result, "Failed to register raw input for the window {0}.", name);
	}

	void Window::BeginFrame()
	{
		// Clear render buffer before each render call.
		graphics->ClearRenderBuffer();
	}

	void Window::EndFrame()
	{
		// Present the frame to the render target.
		graphics->EndFrame();
	}

	Window::~Window()
	{
		// Shutdown the window.
		DestroyWindow(hWnd);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		// Set new title and if it failed throw exception.
		if (SetWindowText(hWnd, newTitle.c_str()) == 0) WINDOW_THROW_EXCEPT("Failed to set the windows title text.");
	}

	void Window::SetDelegateBroadcastEvent(const std::function<void(Delegate&)>& callback)
	{
		BroadcastDel = callback;
	}

	std::optional<int> Window::DispatchMessages() noexcept
	{
		// Perform and dispatch message events from the window.
		MSG message;
		while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))// Hwnd was where nullptr is now...
		{
			// Return the post quit message.
			if (message.message == WM_QUIT)
			{
				return (int)message.wParam;
			}
	
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

	int Window::GetWidth()
	{
		return currWidth;
	}

	int Window::GetHeight()
	{
		return currHeight;
	}

	HWND Window::GetHwnd()
	{
		// Return the window.
		return hWnd;
	}

	void Window::ForceResizeWindow(int newWidth, int newHeight)
	{
		currWidth = newWidth == 0 ? currWidth : newWidth;
		currHeight = newHeight == 0 ? currHeight : newHeight;
		SetWindowPos(hWnd, 0, 0, 0, currWidth, currHeight, SWP_SHOWWINDOW | SWP_NOMOVE);
		graphics->ResizeRenderTargets(currWidth, currHeight);
	}

	void Window::EnableCursor() noexcept
	{
		cursorEnabled = true;
		ShowCursor();
		FreeCursor();
	}

	void Window::DisableCursor() noexcept
	{
		cursorEnabled = false;
		HideCursor();
		ConfineCursor();
	}

	bool Window::CursorEnabled() const noexcept
	{
		return cursorEnabled;
	}

	bool Window::CursorHidden() const noexcept
	{
		return cursorHidden;
	}

	void Window::ConfineCursor() noexcept
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
	}

	void Window::FreeCursor() noexcept
	{
		ClipCursor(nullptr);
	}

	void Window::HideCursor() noexcept
	{
		while (::ShowCursor(FALSE) >= 0);
		cursorHidden = true;
	}

	void Window::ShowCursor() noexcept
	{
		while (::ShowCursor(TRUE) < 0);
		cursorHidden = false;
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
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageRuntime));

			// Custom handling of messages.
			return windowPointer->HandleMessage(hWnd, msg, wParam, lParam);
		}
		else return DefWindowProc(hWnd, msg, wParam, lParam);
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
		// Create delegate array.
		std::vector<Delegate*> delegates;

		// Handle messages being sent from the window class.
		switch (msg)
		{
			// On application closed.
		case WM_CLOSE:
		{
			WindowClosedDelegate closedDel;
			delegates.push_back(&closedDel);
			break;
		}
		// On window resize.
		case WM_SIZE:
		{
			currWidth = LOWORD(lParam);
			currHeight = HIWORD(lParam);
			WindowResizedDelegate resizedDel(currWidth, currHeight);
			delegates.push_back(&resizedDel);
			break;
		}
		case WM_ACTIVATE:
		{
			if (!cursorEnabled)
			{
				if (wParam & WA_ACTIVE)
				{
					ConfineCursor();
					HideCursor();
				}
				else
				{
					FreeCursor();
					ShowCursor();
				}
			}
			break;
		}

		/* KEYBOARD INPUT UPDATE MESSAGES. */
#if KEYBOARD_ENABLED

	// On window focus lost clear the current keySet to prevent ghost key presses.
		case WM_KILLFOCUS:
		{
			input.ResetKeyState();
			KeyboardFocusLostDelegate keyboardStateLost;
			delegates.push_back(&keyboardStateLost);
			break;
		}
		// Pass any keyboard events to the keyboard class for this window.
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			// Only run key pressed event repeatedly if autorepeat is enabled or its the first keyboard press for the key.
			bool isRepeat = (lParam & 0x40000000);
			if (isRepeat) keyRepeatCount++;
			else keyRepeatCount = 0;
			if (!isRepeat || input.IsAutorepeatEnabled())
			{
				auto keycode = static_cast<unsigned char>(wParam);
				input.OnKeyPressed(keycode, keyRepeatCount);
				KeyPressedDelegate keyPressed(keycode, keyRepeatCount);
				delegates.push_back(&keyPressed);
			}
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			auto keycode = static_cast<unsigned char>(wParam);
			input.OnKeyReleased(keycode);
			KeyReleasedDelegate keyReleased(keycode);
			delegates.push_back(&keyReleased);
			break;
		}
		case WM_CHAR:
		{
			auto keycode = static_cast<unsigned char>(wParam);
			input.OnCharPressed(keycode);
			CharPressedDelegate charPressed(keycode);
			delegates.push_back(&charPressed);
			break;
		}

#endif

		/* MOUSE INPUT UPDATE MESSAGES. */
#if MOUSE_ENABLED

		// Capture mouse movement inside and outside of the window if the mouse buttons are held.
		case WM_MOUSEMOVE:
		{
			// Get the mouse position within the window.
			POINTS mousePos = MAKEPOINTS(lParam);

			// If cursor is disabled handle hiding the cursor.
			if (!cursorEnabled)
			{
				if (!input.IsMouseInWindow())
				{
					SetCapture(hWnd);
					input.OnMouseInside();
					HideCursor();
				}
				break;
			}

			// If the mouse is within the window update mouse move as normal.
			if (mousePos.x >= 0 && mousePos.x < currWidth && mousePos.y >= 0 && mousePos.y < currHeight)
			{
				input.OnMouseMove(mousePos.x, mousePos.y);
				MouseMovedDelegate mouseMoved(mousePos.x, mousePos.y);
				delegates.push_back(&mouseMoved);

				// If the mouse is registered as not being in the window toggle it.
				if (!input.IsMouseInWindow())
				{
					SetCapture(hWnd);
					input.OnMouseInside();
					MouseEnteredDelegate mouseEntered;
					delegates.push_back(&mouseEntered);
				}
			}
			// If the mouse has left the window use mouse capture to track movement.
			else
			{
				// If either the left or right mouse button is currently being held down update mouse movement while outside of the window.
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					input.OnMouseMove(mousePos.x, mousePos.y);
					MouseMovedDelegate mouseMoved(mousePos.x, mousePos.y);
					delegates.push_back(&mouseMoved);
				}
				// Otherwise end mouse capture and set the mouse as outside of the window.
				else
				{
					ReleaseCapture();
					input.OnMouseOutside();
					MouseExitDelegate mouseExited;
					delegates.push_back(&mouseExited);
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (!cursorEnabled)
			{
				ConfineCursor();
				HideCursor();
			}
			input.OnMousePressed(EMouseButton::Left);
			MousePressedDelegate mousePressed(EMouseKey::Left);
			delegates.push_back(&mousePressed);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			input.OnMousePressed(EMouseButton::Right);
			MousePressedDelegate mousePressed(EMouseKey::Right);
			delegates.push_back(&mousePressed);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			input.OnMousePressed(EMouseButton::Middle);
			MousePressedDelegate mousePressed(EMouseKey::Middle);
			delegates.push_back(&mousePressed);
			break;
		}
		case WM_LBUTTONUP:
		{
			input.OnMouseReleased(EMouseButton::Left);
			MouseReleasedDelegate mouseReleased(EMouseKey::Left);
			delegates.push_back(&mouseReleased);
			break;
		}
		case WM_RBUTTONUP:
		{
			input.OnMouseReleased(EMouseButton::Right);
			MouseReleasedDelegate mouseReleased(EMouseKey::Right);
			delegates.push_back(&mouseReleased);
			break;
		}
		case WM_MBUTTONUP:
		{
			input.OnMouseReleased(EMouseButton::Middle);
			MouseReleasedDelegate mouseReleased(EMouseKey::Middle);
			delegates.push_back(&mouseReleased);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			// Pass and handle wheel scrolling into the input class.
			const int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
			input.OnMouseWheelDelta(wheelDelta);
			MouseScrolledDelegate mouseScrolled(wheelDelta);
			delegates.push_back(&mouseScrolled);
			break;
		}

		/* RAW MOUSE INPUT MESSAGES. */
		case WM_INPUT:
		{
			// Dont bother reading if raw is enabled.
			if (!input.IsRawMouseInputEnabled()) break;
			
			// Get input size and break if theres an error.
			UINT size;
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1) break;
			rawBuffer.resize(size);

			// Read the input data break if error.
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size) break;

			// Process the raw input data message.
			auto& rawInput = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
			if (rawInput.header.dwType == RIM_TYPEMOUSE && (rawInput.data.mouse.lLastX != 0 || rawInput.data.mouse.lLastY != 0))
			{
				input.OnRawInput(rawInput.data.mouse.lLastX, rawInput.data.mouse.lLastY);
			}
			break;
		}
#endif
		}

		// Disperse delegates from window messages to the application callback event.
		if (!delegates.empty() && BroadcastDel)
		{
			for (Delegate* del : delegates)
			{
				BroadcastDel(*del);
			}
		}

		// Handle any messages not being handled in this function.
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}