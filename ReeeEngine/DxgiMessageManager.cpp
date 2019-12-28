#include "DxgiMessageManager.h"
#include "Window.h"
#include "Graphics.h"
#include <dxgidebug.h>
#include <memory>

// Include linker for dxguid library file.
#pragma comment(lib, "dxguid.lib")

DxgiMessageManager::DxgiMessageManager()
{
	// Define function signature for interface.
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void **);

	// Load dll containing interface library.
	const auto dxgiLibrary = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (dxgiLibrary == nullptr) throw WINDOW_LAST_EXCEPT();

	// Get memory address for dxgiInterface from the dll.
	const auto dxgiInterfaceLoc = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(dxgiLibrary, "DXGIGetDebugInterface")));
	if (dxgiInterfaceLoc == nullptr) throw WINDOW_LAST_EXCEPT();

	// Copy the pointer location for the DxgiInterface from the memory address.
	HRESULT hResult = dxgiInterfaceLoc(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&dxgiInterface));
	GRAPHICS_THROW_NOINFO(hResult);
}

DxgiMessageManager::~DxgiMessageManager()
{
	// Release the pointer on destruction.
	if (dxgiInterface != nullptr) dxgiInterface->Release();
}


void DxgiMessageManager::ClearMessages()
{
	// Set current location within messages vector.
	nextMessage = dxgiInterface->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiMessageManager::GetMessages() const
{
	// Fill vector of strings with each error message after the nextMessage index set in the clearMessages function.
	std::vector<std::string> messages;

	// Loop through each stored message after nextMessages up to numberOfMessages.
	const auto numberOfMessages = dxgiInterface->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = nextMessage; i < numberOfMessages; i++)
	{
		// Get the size of current message in bytes
		HRESULT hResult;
		SIZE_T messageLength;
		hResult = dxgiInterface->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
		GRAPHICS_THROW_NOINFO(hResult);

		// Allocate memory in bytes for copying the message pointer.
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto messagePointer = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		
		// get the message and push its description into the vector
		hResult = dxgiInterface->GetMessage(DXGI_DEBUG_ALL, i, messagePointer, &messageLength);
		GRAPHICS_THROW_NOINFO(hResult);
		messages.emplace_back(messagePointer->pDescription);
	}

	// Return messages as a vector of strings.
	return messages;
}