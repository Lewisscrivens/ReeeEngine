#include "GraphicsException.h"
#include "Graphics.h"
#include "DXErrors/dxerr.h"
#include <sstream>

// Initialize HResultException when created.
GraphicsException::HResultException::HResultException(int lineNo, const char* fileName, HRESULT hResult, std::vector<std::string> messages) noexcept : GException(lineNo, fileName), hResult(hResult)
{
	// For each message in the messages list join then together into a larger message string.
	for (auto i = 0; i < messages.size(); i++)
	{
		// For each message add to message string and make a new line if its not the last message.
		message += messages[i];
		if (i < messages.size()) message.push_back('\n');
	}
}

const char* GraphicsException::HResultException::what() const noexcept
{
	// Format error exception into string.
	std::ostringstream formatedError;
	formatedError << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Error Info] " << GetErrorInfo() << std::endl;

	// If there is anything in the message string add it to a Further Info section.
	if (!message.empty()) formatedError << "\n[Further Info]\n" << GetGraphicsMessage() << std::endl << std::endl;

	// Add origin string onto the end.
	formatedError << GetOriginString();

	// Add formated error string to the exception buffer.
	exceptionBuffer = formatedError.str();

	// Return the exception buffer.
	return exceptionBuffer.c_str();
}

const char* GraphicsException::HResultException::GetType() const noexcept
{
	return "Reee Graphics Class Exception.";
}

HRESULT GraphicsException::HResultException::GetErrorCode() const noexcept
{
	return hResult;
}

std::string GraphicsException::HResultException::GetErrorString() const noexcept
{
	return DXGetErrorString(hResult);
}

std::string GraphicsException::HResultException::GetGraphicsMessage() const noexcept
{
	return message;
}

std::string GraphicsException::HResultException::GetErrorInfo() const noexcept
{
	char infoChar[512];
	DXGetErrorDescription(hResult, infoChar, sizeof(infoChar));
	return infoChar;
}

const char* GraphicsException::GraphicsDeviceLostException::GetType() const noexcept
{
	return "Reeee Graphics Exception -- Device was removed or lost. DXGI_ERROR_DEVICE_REMOVED.";
}

GraphicsException::DirectException::DirectException(int lineNo, const char* fileName, std::vector<std::string> messages) noexcept : GException(lineNo, fileName)
{
	// For each message in the messages list join then together into a larger message string.
	for (auto i = 0; i < messages.size(); i++)
	{
		// For each message add to message string and make a new line if its not the last message.
		message += messages[i];
		if (i < messages.size()) message.push_back('\n');
	}
}

const char* GraphicsException::DirectException::what() const noexcept
{
	// Format error exception into string.
	std::ostringstream formatedError;
	formatedError << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl << GetOriginString();

	// Add formated error string to the exception buffer. (Convert)
	exceptionBuffer = formatedError.str();

	// Return the exception buffer.
	return exceptionBuffer.c_str();
}

const char* GraphicsException::DirectException::GetType() const noexcept
{
	return "Reee Direct 3D Graphics Exception";
}

std::string GraphicsException::DirectException::GetErrorInfo() const noexcept
{
	return message;
}

