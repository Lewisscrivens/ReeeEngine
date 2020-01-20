#pragma once
#include "ReeeWin.h"
#include "DxgiMessageManager.h"
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "ReeeException.h"

//////////////////////////////////////////////////////
/* Define macros for throwing graphical exceptions. */
//////////////////////////////////////////////////////

/* Creates a HResult Exception from the given HRESULT. */
#define GRAPHICS_EXCEPT_NOINFO(hr) GraphicsException::HResultException(__LINE__, __FILE__, hr)

/* Throws HResultException from the given HRESULT. */
#define GRAPHICS_THROW_NOINFO(hr) if (FAILED(hResult = (hr))) throw GraphicsException::HResultException(__LINE__, __FILE__, hResult)

/* If debug is enabled use the graphics message manager to handle getting messages from the dxgi interface. */
#ifndef DEBUG_ENABLED

/* Creates a HResult Exception using the dxgi interface messages and any found error information from the hresult. */
#define GRAPHICS_EXCEPT_INFO(hr) GraphicsException::HResultException(__LINE__, __FILE__, hr, pGraphicsMessageManager.Get().GetMessages())
/* Checks graphics functions and throws exception if failed. */
#define GRAPHICS_THROW_INFO(hr) if (FAILED(hr)) throw GRAPHICS_EXCEPT_INFO(hr)
/* Throw dxgi error message info only. */
#define GRAPHICS_THROW_DIRECT3D_INFO(call) (call); {auto messages = pGraphicsMessageManager.Get().GetMessages(); if(!messages.empty()) {throw GraphicsException::DirectException( __LINE__,__FILE__,v);}}
/* Creates and returns a graphics device lost exception along with any dxgi interface messages. */
#define GRAPHICS_LOST_EXCEPT(hr) GraphicsException::GraphicsDeviceLostException(__LINE__, __FILE__, hr, pGraphicsMessageManager.Get().GetMessages())

#else

/* If debug is not enabled throw exceptions without the dxgi interface messages. */
#define GRAPHICS_EXCEPT_INFO(hr) GraphicsException::HResultException(__LINE__, __FILE__, hr)
#define GRAPHICS_THROW_INFO(hr) GRAPHICS_THROW_NOINFO(hr)
#define GFX_THROW_INFO_ONLY(call) (call)
#define GRAPHICS_LOST_EXCEPT(hr) GraphicsException::GraphicsDeviceLostException(__LINE__, __FILE__, hr)

#endif

/* Class holding any functionality for throwing graphics based engine exceptions. */
class GraphicsException
{
public:

#if DEBUG_ENABLED
	/* Constructor. */
	void SetMessageManager(Microsoft::WRL::ComPtr<DxgiMessageManager> messageManager);
#endif

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

public:

#ifndef DEBUG_ENABLED
	/* Graphics message manager pointer from graphics class for macros. */
	Microsoft::WRL::ComPtr<DxgiMessageManager> pGraphicsMessageManager;
#endif
};

