#pragma once
#include "ReeeWin.h"
#include "DxgiMessageManager.h"
#include "GraphicsException.h"
#include <d3d11.h>
#include <vector>
#include <wrl.h>

namespace ReeeEngine
{
	//////////////////////////////////////////////////////////////////////////////
	/* Define macros for throwing graphical exceptions from the graphics class. */
	//////////////////////////////////////////////////////////////////////////////

	/* Creates a HResult Exception from the given HRESULT. */
	#define GRAPHICS_EXCEPT_NOINFO(hr) GraphicsException::HrException(__LINE__, __FILE__, hr)
	/* Throws HResultException from the given HRESULT. */
	#define GRAPHICS_THROW_NOINFO(hr) if (FAILED(hResult = (hr))) throw GraphicsException::HrException(__LINE__, __FILE__, hResult)

	/* If debug is enabled use the graphics message manager to handle getting messages from the dxgi interface. */
	#ifndef DEBUG_ENABLED
		/* Creates a HResult Exception using the dxgi interface messages and any found error information from the hresult. */
		#define GRAPHICS_EXCEPT_INFO(hr) GraphicsException::HrException(__LINE__, __FILE__, hr, messageManager.GetMessages())
		/* Checks graphics functions and throws exception if failed. */
		#define GRAPHICS_THROW_INFO(hr) if (FAILED(hr)) throw GRAPHICS_EXCEPT_INFO(hr)
		/* Throw dxgi error message info only. */
		#define GRAPHICS_THROW_INFO_ONLY(call) messageManager.ClearMessages(); (call); {auto v = messageManager.GetMessages(); if (!v.empty()) { throw GraphicsException::DirectException(__LINE__, __FILE__, v); }}
		/* Creates and returns a graphics device lost exception along with any dxgi interface messages. */
		#define GRAPHICS_LOST_EXCEPT(hr) GraphicsException::GraphicsDeviceLostException(__LINE__, __FILE__, hr, messageManager.GetMessages())
	#else
		/* If debug is not enabled throw exceptions without the dxgi interface messages. */
		#define GRAPHICS_EXCEPT_INFO(hr) GraphicsException::HrException(__LINE__, __FILE__, hr)
		#define GRAPHICS_THROW_INFO(hr) GRAPHICS_THROW_NOINFO(hr)
		#define GRAPHICS_THROW_INFO_ONLY(call) (call)
		#define GRAPHICS_LOST_EXCEPT(hr) GraphicsException::GraphicsDeviceLostException(__LINE__, __FILE__, hr)
	#endif

	/* Create and handle graphics device. */
	class Graphics
	{
	public:

		/* Constructor. Initialize and create the device, swap chain and context objects.
		 * NOTE: Disable copying and moving of a graphics class as we do not need that functionality. */
		Graphics(HWND hWnd);
		Graphics(const Graphics&) = delete;
		Graphics& operator = (const Graphics&) = delete;
		~Graphics() = default;

		/* End frame function. */
		void EndFrame();

		/* Clears the render buffer state to a given color.
		 * NOTE: By default with no input it is cleared to black. */
		void ClearRenderBuffer(float r = 0.0f, float g = 0.0f, float b = 0.0f) noexcept;

		/* Draw a cube at a given angle. */
		void DrawCube(float angle, float x, float y);

	private:

		/* Create graphics device variables. */
		/* NOTE: ComPtr handles releasing after application shutdown making destructor's unnecessary. */
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencil;

	public:

		/* Message management class for handling DirectX messages from its message buffer. */
		DxgiMessageManager messageManager;
	};
}