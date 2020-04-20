#pragma once
#include <ReeeEngine.h>

/* Extended application class for this app from the ReeeEngine. */
class EngineApp : public ReeeEngine::Application
{
public:

	/* Constructor and Destructor. */
	EngineApp();
	~EngineApp();

	/* Ran at the start of the engine initialization. */
	virtual void Init() override;

private:

	/* Ran on engine frame. */
	virtual void Tick() override;
};

/* Define create app function to return this app to the engine entry point. */
ReeeEngine::Application* ReeeEngine::CreateApp()
{
	return new EngineApp();
}

