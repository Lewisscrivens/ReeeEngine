#pragma once
#include <ReeeEngine.h>

/* Extended application class for this app from the ReeeEngine. */
class EngineApp : public ReeeEngine::Application
{
public:

	/* Constructor and Destructor. */
	EngineApp();
	~EngineApp();
};

/* Define create app function to return this app to the engine entry point. */
ReeeEngine::Application* ReeeEngine::CreateApp()
{
	return new EngineApp();
}

