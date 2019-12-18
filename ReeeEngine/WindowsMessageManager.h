#pragma once
#include <Windows.h>
#include <unordered_map>

/* Class to handle logging messages from a window. */
class WindowsMessageManager
{
public:

	/* Constructor */
	WindowsMessageManager();

	/* Operator to format messages to a string. */
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const noexcept;

private:

	/* Map of messages for opened window. */
	std::unordered_map<DWORD, std::string> messageMap;

};

