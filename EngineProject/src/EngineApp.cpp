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
	Application::Init();


	REEE_LOG(Log, "Intialised Application....");
}

void EngineApp::Tick(float deltaTime)
{
	Application::Tick(deltaTime);


}
