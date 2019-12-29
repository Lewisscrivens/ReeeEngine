#pragma once
#include "ReeeWin.h"
#include <wrl.h>
#include <vector>
#include <wrl.h>
#include <dxgidebug.h>

/* DXGI Interface manager for returning messages from debug layer. */
class DxgiMessageManager
{
public:

	/* Constructor and destructor for adding and releasing the Dxgi interface. */
	DxgiMessageManager();
	~DxgiMessageManager() = default;

	/* Dont allow this class to be set or copied after creation. */
	DxgiMessageManager(const DxgiMessageManager&) = delete;
	DxgiMessageManager& operator = (const DxgiMessageManager&) = delete;

	/* Clear and Get messages. Clears all old messages, returns list of messages ran after clear messages was last ran. */
	void ClearMessages();
	std::vector<std::string> GetMessages() const;

	unsigned long long nextMessage = 0u;// Next message to save location in message vector.
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> dxgiInterface;// Pointer to the dxgi interface.
};

