#include "WindowException.h"
#include <sstream>

namespace ReeeEngine
{
	// Initialize window exception on creation.
	WindowException::HrException::HrException(int line, const char* file, HRESULT hResult) noexcept : Exception(line, file), hResult(hResult) {}

	const char* WindowException::HrException::what() const noexcept
	{
		// Create the exception string for the window using a string steam.
		std::ostringstream exceptionString;
		exceptionString << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[INFO] " << GetErrorInfo() << std::endl << GetOriginString();

		// Set the exception buffer to last known exception string.
		exceptionBuffer = exceptionString.str();
		return exceptionBuffer.c_str();
	}

	const char* WindowException::HrException::GetType() const noexcept
	{
		return "Reee Window Exception";
	}

	HRESULT WindowException::HrException::GetErrorCode() const noexcept
	{
		return hResult;
	}

	std::string WindowException::HrException::GetErrorInfo() const noexcept
	{
		// Format the message from the hResult.
		char* messageBuff = nullptr;
		DWORD messageID = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&messageBuff), 0, nullptr);

		// If the message length is 0 its unidentified otherwise return the message/string.
		if (messageID == 0) return "Unknown error information.";

		// Copy string from message buffer before freeing it so it can be later returned.
		std::string errorString = messageBuff;
		LocalFree(messageBuff);
		return errorString;
	}

	const char* WindowException::NoGraphicsException::GetType() const noexcept
	{
		return "Reee No Graphics Exception";
	}

	std::string WindowException::Exception::TranslateErrorMessage(HRESULT hResult) noexcept
	{
		char* messageBuffer = nullptr;

		// Allocate memory for formated message buffer.
		const DWORD messageLength = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

		// If the message has no length, the error has no description message.
		if (messageLength == 0) return "No message found from error code.";

		// Copy string from message buffer.
		std::string errorMessage = messageBuffer;

		// Release the message buffer after use.
		LocalFree(messageBuffer);

		// Return the error.
		return errorMessage;
	}
}