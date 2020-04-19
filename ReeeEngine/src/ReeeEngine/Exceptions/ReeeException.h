#pragma once
#include "../Globals.h"
#include <exception>
#include <string>

namespace ReeeEngine
{
	/* Reee Engine exception parent class for implementing exceptions within classes. */
	class ReeeException : public std::exception
	{
	public:

		/* Constructor. */
		ReeeException(int line, const char* file) noexcept;

		/* Exception override function to return exception char. */
		const char* what() const noexcept override;

		/* Virtual function that returns the type of exception. */
		virtual const char* GetType() const noexcept;

		/* Return the line number the exception was thrown on from the file. */
		int GetLineNumber() const noexcept;

		/* Return the file name for where the exception was thrown. */
		const std::string& GetFileName() const noexcept;

		/* Returns the exception type, file name and line number formated within one string. */
		std::string GetOriginString() const noexcept;

	private:

		/* Exception variables. */
		int lineNo; // Line number of the file its was thrown.
		std::string fileName; // File that the exception was thrown.

	protected:

		/* Buffer string for exceptions. Accessible from children classes. */
		mutable std::string exceptionBuffer;
	};
}