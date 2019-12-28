#include "ReeeException.h"
#include <sstream>

// Default initializer.
ReeeException::ReeeException(int line, const char* file) noexcept : lineNo(line), fileName(file) {}

const char* ReeeException::what() const noexcept
{
	// Create the exception string using a string steam.
	std::ostringstream exceptionString;
	exceptionString << GetType() << std::endl << GetOriginString();

	// Set the exception buffer to last known exception string.
	exceptionBuffer = exceptionString.str();
	return exceptionBuffer.c_str(); 
}

const char* ReeeException::GetType() const noexcept
{
	return "Reee Exception";
}

int ReeeException::GetLineNumber() const noexcept
{
	return lineNo;
}

const std::string& ReeeException::GetFileName() const noexcept
{
	return fileName;
}

std::string ReeeException::GetOriginString() const noexcept
{
	// Create and format a string to return then return it.
	std::ostringstream originString;
	originString << "[File] " << fileName << std::endl << "[LineNo] " << lineNo;
	return originString.str();
}
