#pragma once
#include "ReeeWin.h"
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "ReeeException.h"

/* Holds the exception classes/structures for the graphics class. */
class GraphicsException
{
public:

	/* Base graphics exception class. */
	class GException : public ReeeException
	{
		// Default graphics exception.
		using ReeeException::ReeeException;
	};

	/* HResult exceptions for the graphics class to throw errors using the HResult and the dxgi interface if DEBUG_ENABLED == 1 */
	class HResultException : public GException
	{
	public:

		/* HResult exception where error information is derived from the hResult. */
		HResultException(int lineNo, const char* fileName, HRESULT hResult, std::vector<std::string> messages = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetGraphicsMessage() const noexcept;
		std::string GetErrorInfo() const noexcept;

	private:

		HRESULT hResult;
		std::string message;
	};

	/* Direct 3D exception to pass messages from the dxgi interface only. */
	class DirectException : public GException
	{
	public:

		/* Exception function overrides to change whats returned. */
		DirectException(int lineNo, const char* fileName, std::vector<std::string> messages) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;

	private:

		/* Stored exception message. */
		std::string message;
	};

	/* On device removed or lost exception. */
	class GraphicsDeviceLostException : public HResultException
	{

		/* Use functions from HResultException. */
		using HResultException::HResultException;

	public:

		/* Override the type to return device lost message. */
		const char* GetType() const noexcept override;

	private:

		/* Reason that the graphics device was lost? */
		std::string exceptionInfo;
	};
};

