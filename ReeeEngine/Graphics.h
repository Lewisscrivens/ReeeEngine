#pragma once
#include "ReeeWin.h"
#include "DxgiMessageManager.h"
#include <d3d11.h>
#include <vector>
#include "ReeeException.h"

/* Define macros for throwing graphical exceptions. */
#define GRAPHICS_EXCEPT_NOINFO(hr) Graphics::HResultException(__LINE__, __FILE__, hr)
#define GRAPHICS_THROW_NOINFO(hr) if (FAILED(hResult = (hr))) throw Graphics::HResultException(__LINE__, __FILE__, hResult)

/* If debug is enabled use the graphics message manager to handle getting messages from the dxgi interface. */
#ifndef DEBUG_ENABLED
#define GRAPHICS_EXCEPT_INFO(hr) Graphics::HResultException(__LINE__, __FILE__, hr, graphicsMessageManager.GetMessages())
#define GRAPHICS_THROW_INFO(hr) if (FAILED(hr)) throw GRAPHICS_EXCEPT_INFO(hr)
#define GRAPHICS_LOST_EXCEPT(hr) Graphics::GrpahicsDeviceLostException(__LINE__, __FILE__, hr, graphicsMessageManager.GetMessages())
#else
#define GRAPHICS_EXCEPT_INFO(hr) Graphics::HResultException(__LINE__, __FILE__, hr)
#define GRAPHICS_THROW_INFO(hr) GRAPHICS_THROW_NOINFO(hr)
#define GRAPHICS_LOST_EXCEPT(hr) Graphics::GrpahicsDeviceLostException(__LINE__, __FILE__, hr)
#endif


/* Create and handle graphics device. */
class Graphics
{
public:

	/* Graphical exception class for throwing graphics exceptions. */
	class GraphicsException : public ReeeException
	{
		// Default graphics exception.
		using ReeeException::ReeeException;
	};

	/* HResult exceptions for the graphics class. */
	class HResultException : public GraphicsException
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

	/* On device removed or lost exception. */
	class GrpahicsDeviceLostException : public HResultException
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
	
	/* Constructor. Initialize and create the device, swap chain and context objects.
	 * NOTE: Disable copying and moving of a graphics class as we do not need that functionality. */
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;

	/* Destructor. */
	~Graphics();

	/* End frame function. */
	void EndFrame();

	/* Clears the render target to a given color.
	 * NOTE: By default with no input it is cleared to black. */
	void ClearRenderTarget(float r = 0.0f, float g = 0.0f, float b = 0.0f) noexcept;

private:

	/* Create graphics device variables. */
	ID3D11Device* device = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* renderTarget = nullptr;

	/* If the debug layer is enabled, create an info manager. */
#ifndef DEBUG_ENABLED
	DxgiMessageManager graphicsMessageManager;
#endif
};