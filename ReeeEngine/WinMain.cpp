#include <Windows.h>
#include "WindowsMessageManager.h"

/* Windows function called when messages are passed from the hWnd window.
 * Messages can be found here: https://wiki.winehq.org/List_Of_Windows_Messages */
LRESULT CALLBACK WinProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Log each message that is called while the window is open.
	static WindowsMessageManager messageManager;
	// Output message as debug string after formating using the operator function in windows message manager.
	OutputDebugString(messageManager(msg, lParam, wParam).c_str());

	// Handle specific messages using switch.
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(100);// Exit code one on close.
	break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "F HAS BEEN PRESSED.");
		}
	break;
	}

	// Return and process message.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*	GAME LOOP
	Update loop -> Proccess Input -> Process Game Update -> Render -> Repeat. */
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPreviewInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create window class name.
	const auto windowClassName = "ReeeEditor";

	// Configure the window and register it.
	WNDCLASSEX windowsClass = { 0 };
	windowsClass.cbSize = sizeof(windowsClass);
	windowsClass.style = CS_OWNDC;
	windowsClass.lpfnWndProc = WinProcedure; // Set windowProcedure function for this window.
	windowsClass.cbClsExtra = 0;
	windowsClass.cbWndExtra = 0;
	windowsClass.hInstance = hInstance;
	windowsClass.hIcon = nullptr;
	windowsClass.hCursor = nullptr;
	windowsClass.hbrBackground = nullptr;
	windowsClass.lpszMenuName = nullptr;
	windowsClass.lpszClassName = windowClassName;
	windowsClass.hIconSm = nullptr;
	RegisterClassEx(&windowsClass);

	// Create an instance of the window and dont return until it is closed or an exception is hit.
	HWND hWnd = CreateWindowEx(
		0, windowClassName,
		"ReeeEditor",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// Show the window after its creation.
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	// Handle the message cue for the created window.
	MSG message;
	BOOL appResult;
	while ((appResult = GetMessage(&message, hWnd, 0, 0)) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message); // Pass message to the windows processor for this window.
	}

	// If the while loop is broken close the window with the relevant message.
	if (appResult == -1) return -1;
	else return message.wParam;// wParam NOTE: Exit code given in PostQuitMessage function.
}