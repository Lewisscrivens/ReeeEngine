#include "Engine.h"

/* Entry state for the App. Create a window and enter while loop to receive and dispatch messages for created window class. */
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPreviewInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Catch any exceptions while updating the engine.
	try
	{
		return Engine{}.Update();
	}
	catch (const ReeeException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Default Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No information found", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	// Return.
	return -1;
}