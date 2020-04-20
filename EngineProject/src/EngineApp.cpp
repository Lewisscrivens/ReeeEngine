#include "EngineApp.h"
#include "ReeeEngine/ReeeLog.h"
using namespace ReeeEngine;

EngineApp::EngineApp()
{
	//...
}

EngineApp::~EngineApp()
{
	//...
}

void EngineApp::Init()
{
	Application::Init();// Call parent important...

	REEE_LOG(Log, "Intialised Application....");
}

void EngineApp::Tick()
{
	Application::Tick();// Call parent important...


}
