#pragma once
#include "ReeeWin.h"
#include "ReeeException.h"
#include "WindowsInput.h"
#include <optional>
#include <memory>
#include "Graphics.h"

namespace ReeeEngine
{
	/* Holds the exception classes/structures for the window class. */
	class WindowException
	{
	public:

		/* Window exception class to hold static error code translate function. */
		class Exception : public ReeeException
		{
			using ReeeException::ReeeException;

		public:

			/* Returns formated error code string from HRESULT exception. */
			static std::string TranslateErrorMessage(HRESULT hResult) noexcept;

		};

		/* Create another error exception for the window for HRESULT failures. */
		class HrException : public Exception
		{
		public:

			/* Constructor. */
			HrException(int line, const char* file, HRESULT hResult) noexcept;

			/* Exception message override. */
			const char* what() const noexcept override;

			/* Type of message override. */
			virtual const char* GetType() const noexcept override;

			/* Gets the hResult from an error code. */
			HRESULT GetErrorCode() const noexcept;

			/* Returns a string describing the error. Translates this exceptions hResult to a string. */
			std::string GetErrorInfo() const noexcept;

		private:

			/* Pointer to the hResult. */
			HRESULT hResult;
		};

		/* No graphics exception class to indicate what exception was hit. */
		class NoGraphicsException : public Exception
		{

			/* Use exception classes constructor. */
			using Exception::Exception;

		public:

			/* Return the type of exception in this case a no graphics exception. */
			const char* GetType() const noexcept override;
		};
	};
}
