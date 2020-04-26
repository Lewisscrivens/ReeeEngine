#pragma once

#ifdef PLATFORM_WINDOWS
#include "Windows/ReeeWin.h"
#include "ReeeLog.h"
	
/* Needs creating in each sub application to this engine. */
extern ReeeEngine::Application* ReeeEngine::CreateApp();

/* Entry state for the Windows Application. Create a window and enter while loop to receive and dispatch messages for created window class. */
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPreviewInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Initalise logging system.
	ReeeEngine::ReeeLog::InitaliseLogging();

	// Start app.
	auto application = ReeeEngine::CreateApp();
	return application->Start();
}

#endif
